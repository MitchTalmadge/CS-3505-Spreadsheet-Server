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

void data_container::new_client(int socket_id, std::string spreadsheet) {
  // Update the mappings according to the new client.
  std::lock_guard<std::mutex> lock_spreadsheet(spreadsheet_to_sockets_mutex);
  std::lock_guard<std::mutex> lock_sockets(sockets_to_spreadsheet_mutex);
  
  spreadsheet_to_sockets[spreadsheet].push_back(socket_id);
  sockets_to_spreadsheet[socket_id] = spreadsheet;
}

void data_container::new_inbound_packet(inbound_packet &packet) {
  std::lock_guard<std::mutex> lock_sockets(sockets_to_spreadsheet_mutex);
  std::lock_guard<std::mutex> lock_spreadsheet_in(inbound_messages_mutex);

  // Place packet onto queue for given spreadsheet.
  inbound_messages[sockets_to_spreadsheet[packet.get_socket_id()]].push(&packet);
}

/*
Allow the model to get a message for the given string spreadsheet.
 */
inbound_packet *data_container::get_inbound_packet(std::string spreadsheet) {
  std::lock_guard<std::mutex> lock(inbound_messages_mutex);

  // Grab and remove top packet.
  inbound_packet* packet_in = inbound_messages[spreadsheet].front();
  inbound_messages[spreadsheet].pop();

  return packet_in;
}

/*
Send a new message to the specified socket.
Called by the main controller during registration.
 */
void data_container::new_outbound_packet(int socket_id, outbound_packet &packet) {
  std::lock_guard<std::mutex> lock(outbound_messages_mutex);

  auto &queue = outbound_messages[socket_id];
  queue.push(&packet);
}

/*
Send a new message to all the attached sockets for the given spreadsheet.
Called by the spreadsheet models.
 */
void data_container::new_outbound_packet(std::string spreadsheet, outbound_packet &packet) {
  std::lock_guard<std::mutex> lock_spreadsheets(spreadsheet_to_sockets_mutex);
  std::lock_guard<std::mutex> lock_outbound(outbound_messages_mutex);

  // Forward packet to all connected sockets for that spreadsheet.
  std::vector<int> sockets = spreadsheet_to_sockets[spreadsheet];
  for(auto client = sockets.begin(); client != sockets.end(); ++client) {
    outbound_messages[(*client)].push(&packet);
  }
}

/*
Allow for socket to grab an outbound message to be sent to client.
 */
outbound_packet *data_container::get_outbound_packet(int socket_id) {
  std::lock_guard<std::mutex> lock(outbound_messages_mutex);

  auto &queue = outbound_messages[socket_id];

  if (queue.empty()) {
    return nullptr;
  }

  auto packet = queue.front();
  queue.pop();

  return packet;
}
