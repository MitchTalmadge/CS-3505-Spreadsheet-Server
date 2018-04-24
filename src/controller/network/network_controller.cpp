/*
network_controller.cpp

Network Controller handles maintaining connections and communication with connected
clients.
 */

#include "network_controller.h"
#include <csignal>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <boost/regex.hpp>
#include <boost/chrono.hpp>
#include <boost/algorithm/string.hpp>
#include <model/packet/inbound/inbound_packet_factory.h>
#include <model/packet/outbound/outbound_ping_response_packet.h>
#include <model/packet/outbound/outbound_ping_packet.h>
#include <model/packet/outbound/outbound_disconnect_packet.h>
#include <model/packet/inbound/inbound_unfocus_packet.h>
#include <iostream>

const std::string network_controller::EOT = std::string(1, '\3');

network_controller::network_controller() {
  // Ignore SIG_PIPE signals so we don't die when someone disconnects.
  signal(SIGPIPE, SIG_IGN);
}

/*
Create a new network controller. Set up any variables as necessary.
 */
network_controller &network_controller::get_instance() {
  static network_controller instance;
  return instance;
}

/**
 * Helper to send a disconnect packet to the specified socket and close that socket.
 */
void disconnect_socket(int socket_id) {
  // Send disconnect message.
  outbound_disconnect_packet* packet = new outbound_disconnect_packet();
  std::string raw_message = packet->get_raw_message();
  const char *msg = raw_message.c_str();

  // Dispose of packet.
  delete packet;

  // Set socket to blocking so we don't close the socket before the message is sent.
  bool block_socket = network_controller::set_socket_block_state(socket_id, true);

  if(!block_socket) {
    std::cout << "Socket was not set to blocking. Disconnect message not sent. Socket closing." << std::endl;
  }

  write(socket_id, msg, strlen(msg) * sizeof(char));

  // Shut down the socket.
  close(socket_id);
}

/*
Work loop for the network controller, where it listens in on the provided socket 
and checks to see if it needs to send a message out on the given socket.

Note:
Because our sockets are non-blocking, this loop will check if a message has arrived
but will NOT block until one arrives. This allows it to fall through and check if
there is a message to be sent out. We use a sleep to keep this from overworking the 
server.
 */
void network_controller::socket_work_loop(int socket_id) {
  std::cout << "Listening on socket " << socket_id << std::endl;

  // Ping timer counts down from 10000 milliseconds and can be checked to trigger
  // a ping send.
  int ping_timer = network_controller::ping_time;

  // Ping timeout timer counts down from 60000 milliseconds and can be checked to 
  // trigger a ping send.
  int ping_timeout_timer = network_controller::ping_timeout;

  char buffer[1024] = {0};
  while (true) {
    // Check if a message has arrived since last time.
    read(socket_id, &buffer, 1024);

    if (strlen(buffer) != 0) {
      std::vector<std::string> split;
      boost::split(split, buffer, boost::is_any_of(network_controller::EOT));

      // Note, we only go to length - 1, since we don't want anything that doesn't end in EOT.
      for (int index = 0; index < split.size()-1; ++index) {
	std::string message = split[index] + network_controller::EOT;

	std::cout << "Message from client: " << std::endl;
	std::cout << message << std::endl;

	// Parse the message as an inbound packet.
	auto packet = inbound_packet_factory::from_raw_message(socket_id, message);

	// Check if packet was parsed.
	if (!packet) {
	  continue;
	}

	// Handle parsed packet.
	switch (packet->get_packet_type()) {

	case inbound_packet::PING: {
	  data_container_.new_outbound_packet(packet->get_socket_id(), *new outbound_ping_response_packet());

	  // Dispose of packet.
	  delete packet;

	  break;
	}
	case inbound_packet::PING_RESPONSE: {
	  // Client has responded to our most recent Ping. Reset the ping_timeout_timer.
	  ping_timeout_timer = network_controller::ping_timeout;

	  // Dispose of packet.
	  delete packet;

	  break;
	}
	case inbound_packet::DISCONNECT: {
	  std::cout << "Disconnecting client on socket " << socket_id << std::endl;

	  // Shut down the socket.
	  close(socket_id);

	  // Clean up resources for this socket.
	  data_container_.remove_socket(socket_id);

	  // Create an unfocus message for this socket, to unfocus whatever it was looking at in other still connected clients.
	  data_container_.new_inbound_packet(*new inbound_unfocus_packet(socket_id, "unfocus " + network_controller::EOT));

	  // Dispose of packet.
	  delete packet;

	  // Return which will end this thread.
	  return;
	}
	default: {
	  data_container_.new_inbound_packet(*packet);
	  break;
	}
	}
      }
      // Clear the buffer.
      memset(buffer, 0, 1024);
    }

    // Read and send from outbound message queue as necessary.
    auto packet = data_container_.get_outbound_packet(socket_id);

    // If a message is there to be sent, send it!
    if (packet) {
      std::string raw_message = packet->get_raw_message();
      const char *msg = raw_message.c_str();

      // Dispose of packet.
      delete packet;

      std::cout << "Sending message to client: " << msg << std::endl;

      write(socket_id, msg, strlen(msg) * sizeof(char));
    }

    // Check if either timer has gone off and handle accordingly.
    if (ping_timer < 0) {
      std::cout << "Sending ping." << std::endl;

      // Create and send a ping packet.
      data_container_.new_outbound_packet(socket_id, *new outbound_ping_packet());

      // Reset the ping timer.
      ping_timer = network_controller::ping_time;
    }
    if (ping_timeout_timer < 0) {
      // Client hasn't responded in 60 seconds, so we disconnect them.
      std::cout << "Disconnecting client on socket " << socket_id << " due to ping timeout." << std::endl;

      // Send disconnect and close socket.
      disconnect_socket(socket_id);

      // Clean up resources for this socket.
      data_container_.remove_socket(socket_id);

      // Return which will end this thread.
      return;
    }

    try {
      // Briefly sleep to prevent this from choking machine resources.
      boost::this_thread::sleep_for(boost::chrono::milliseconds{10});

      ping_timer -= 10;
      ping_timeout_timer -= 10;
    } catch (boost::thread_interrupted interruption) {
      std::cout << "Shutting down client on socket " << socket_id << std::endl;

      // Send disconnect and close socket.
      disconnect_socket(socket_id);

      // Return which will end this thread.
      return;
    }
  }
}

/*
When a new client connects, determine whether to create a new queue for a new
spreadsheet, then setup a loop listening on the new socket for communications.
 */
void network_controller::start_work(int socket_id) {
  bool set_non_blocking = network_controller::set_socket_block_state(socket_id, false);

  // If something went wrong, close the socket.
  if (!set_non_blocking) {
    std::cout << "Error in setting up socket. Closing connection..." << std::endl;
    shutdown(socket_id, SHUT_RDWR);
  }

  // Start a thread that can listen in on the given socket.
  boost::thread *work_thread = new boost::thread(&network_controller::socket_work_loop, this, socket_id);

  // Add thread to list of active threads.
  worker_threads_.push_back(work_thread);
}

/*
Clean up and shut down.
 */
network_controller::~network_controller() {
  // Interrupt and delete all our threads.
  std::cout << "Shutting down worker threads." << std::endl;

  for (auto &thread : worker_threads_) {
    thread->interrupt();
    thread->join();
    delete thread;
  }
}

bool network_controller::set_socket_block_state(int socket_id, bool blocking) {
  // Get current socket flags.
  int flags = fcntl(socket_id, F_GETFL, 0);

  // Make sure we got flags correctly.
  if (flags == -1) return false;

  // Now set the socket to non-blocking.
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);

  return (fcntl(socket_id, F_SETFL, flags) == 0) ? true : false;
}
