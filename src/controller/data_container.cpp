/*
data_container.cpp

Data container holds shared data between the network controller and the model.
This is meant to consolidate the areas where multiple threads will need to be 
able to access shared data.
 */

#include "data_container.h"
#include <iostream>

data_container &data_container::get_instance() {
  static data_container instance; // Initialized on first-use.
  return instance;
}

void data_container::register_socket(int socket_id) {
  std::lock_guard<std::mutex> lock(disconnected_clients_mutex_);

  // This socket is no longer disconnected.
  disconnected_clients_.erase(socket_id);
}

void data_container::remove_socket(int socket_id) {
  std::lock_guard<std::mutex> lock_outbound(outbound_packets_mutex_);
  std::lock_guard<std::mutex> lock_disconnected(disconnected_clients_mutex_);

  disconnected_clients_.insert(socket_id);

  // Free up memory used by that socket.
  std::queue<outbound_packet *> &packets = outbound_packets_[socket_id];

  while (!packets.empty()) {
    outbound_packet *packet = packets.front();
    packets.pop();
    delete packet;
  }

  // Remove the outbound queue.
  outbound_packets_.erase(socket_id);
}

void data_container::new_inbound_packet(inbound_packet &packet) {
  std::lock_guard<std::mutex> lock(inbound_packets_mutex_);

  // Place packet onto queue for given spreadsheet.
  inbound_packets_.push(&packet);
}

data_container::~data_container() {
  // Free all packets in outbound packet queues.
  for (auto it = outbound_packets_.begin(); it != outbound_packets_.end(); ++it) {
    std::queue<outbound_packet *> &packets = it->second;

    while (!packets.empty()) {
      outbound_packet *packet = packets.front();
      packets.pop();
      delete packet;
    }
  }

  // Clear out the inbound packet queue.
  while (!inbound_packets_.empty()) {
    inbound_packet *packet = inbound_packets_.front();
    inbound_packets_.pop();
    delete packet;
  }
}

/*
Allow the model to get a message for the given string spreadsheet.
 */
inbound_packet *data_container::get_inbound_packet() {
  std::lock_guard<std::mutex> lock(inbound_packets_mutex_);

  // Grab and remove top packet.
  if (!inbound_packets_.empty()) {
    inbound_packet *packet_in = inbound_packets_.front();
    inbound_packets_.pop();
    return packet_in;
  } else {
    return nullptr;
  }

}

/*
Send a new message to the specified socket.
Called by the main controller during registration.
 */
bool data_container::new_outbound_packet(int socket_id, outbound_packet &packet) {
  std::lock_guard<std::mutex> lock_outbound(outbound_packets_mutex_);
  std::lock_guard<std::mutex> lock_disconnected(disconnected_clients_mutex_);

  // If the asked for socket has closed, tell spreadsheet_controller.
  auto disconnect_it = disconnected_clients_.find(socket_id);
  if (disconnect_it != disconnected_clients_.end()) {
    disconnected_clients_.erase(socket_id);
    return false;
  }

  auto &queue = outbound_packets_[socket_id];
  queue.push(&packet);
  return true;
}

/*
Allow for socket to grab an outbound message to be sent to client.
 */
outbound_packet *data_container::get_outbound_packet(int socket_id) {
  std::lock_guard<std::mutex> lock(outbound_packets_mutex_);

  auto &queue = outbound_packets_[socket_id];

  if (queue.empty()) {
    return nullptr;
  }

  auto packet = queue.front();
  queue.pop();

  return packet;
}
