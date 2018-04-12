/*
network_controller.cpp

Network Controller handles maintaining connections and communication with connected
clients.
 */

#include "network_controller.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <boost/ref.hpp>
#include <boost/regex.hpp>
#include <boost/chrono.hpp>
#include <string.h>

/*
Create a new network controller. Set up any variables as necessary.
 */
network_controller::network_controller(data_container& data_container)
{
  data = data_container;
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
void network_controller::socket_work_loop(int socket_id, std::function<std::string(int, std::string)> callback)
{
  std::cout << "Listening on socket " << socket_id << std::endl;

  char buffer[1024] = {0};
  while(true)
  {
    // Check if a message has arrived since last time.
    read(socket_id, &buffer, 1024);

    if (strlen(buffer) != 0)
    {
      std::cout << "Message from client: " << std::endl;
      std::cout << buffer << std::endl;

      // Send messages to the main_controller who decides where to send them next.
      callback(socket_id, buffer);

      // Clear the buffer.
      memset(buffer, 0, 1024);
    }

    // Read and send from outbound message queue as necessary.
    std::string message = data.get_outbound_message(socket_id);

    // If a message is there to be sent, send it!
    if (message.length() != 0)
    {
      const char * msg = message.c_str();

      std::cout << "Message to send to client: " << msg << std::endl;

      write(socket_id, msg, strlen(msg) * sizeof(char));
    }

    // Briefly sleep to prevent this from choking machine resources.
    boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
  }
}

/*
Helper to set the socket associated with the provided socket id to non-blocking
to allow our work loop to work correctly.
 */
bool set_socket_non_blocking(int socket_id)
{
  // Get current socket flags.
  int flags = fcntl(socket_id, F_GETFL, 0);

  // Make sure we got flags correctly.
  if (flags == -1) return false;

  // Now set the socket to non-blocking.
  flags = flags | O_NONBLOCK;
  return (fcntl(socket_id, F_SETFL, flags) == 0) ? true : false;
}

/*
When a new client connects, determine whether to create a new queue for a new
spreadsheet, then setup a loop listening on the new socket for communications.
 */
void network_controller::start_work(int socket_id, std::function<std::string (int, std::string)> callback)
{
  bool set_non_blocking = set_socket_non_blocking(socket_id);

  // If something went wrong, close the socket.
  if (!set_non_blocking)
  {
    std::cout << "Error in setting up socket. Closing connection..." << std::endl;
    shutdown(socket_id, SHUT_RDWR);
  }

  // Start a thread that can listen in on the given socket.
  boost::thread work_thread(&network_controller::socket_work_loop, this, socket_id, callback);
}

/*
Clean up and shut down.
 */
void network_controller::shut_down()
{
  // TODO: shutdown each socket?
}
