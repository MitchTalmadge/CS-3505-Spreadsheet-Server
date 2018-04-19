/*
main_controller.cpp

Main Controller:
Coordinate model/client activity, specifically:
- Create our network controller to handle communication from/with connected clients.
- Create models when network controller signals as such.
 */

#include "main_controller.h"
#include <iostream>
#include <boost/regex.hpp>
#include <model/packet/inbound/inbound_packet_factory.h>
#include <model/packet/outbound/outbound_ping_response_packet.h>

main_controller &main_controller::get_instance() {
  static main_controller instance;
  return instance;
}

/*
Clean up the main controller. Specifically, tell the network controller
and models to shut down.
 */
void main_controller::shut_down() {
  spreadsheet_controller::shut_down();
}

/*
When a new client is connected for the given spreadsheet, forward it to our network controller
who will control the interactions with it.
 */
void main_controller::handle_client(int socket_id) {
  // Forward the new socket to the network controller who sets up a communication loop.
  network_controller_.start_work(socket_id);
}

/*
Handle a message arriving from a client. Specifically:
- If a register, queue an outgoing message with list of spreadsheets.
- If a load, determine if new model necessary.
- Handle disconnects. (How?)
- Forward edits into the corresponding spreadsheets inbound queue.
 */
void main_controller::message_callback(int socket_src, std::string message) {
  auto packet = inbound_packet_factory::from_raw_message(socket_src, message);

  // Check if packet was parsed.
  if (!packet) {
    return;
  }

  // Handle parsed packet.
  switch (packet->get_packet_type()) {

    case inbound_packet::PING: {
      data_container_.new_outbound_packet(packet->get_socket_id(), *new outbound_ping_response_packet());
      break;
    }
    default: {
      data_container_.new_inbound_packet(*packet);
      return;
    }
  }

  // Dispose of packet.
  delete packet;
}
