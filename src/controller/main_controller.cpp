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
#include <model/packet/inbound/inbound_load_packet.h>
#include <model/packet/outbound/outbound_connect_accepted_packet.h>

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
  network_controller_.start_work(socket_id, std::bind(&main_controller::message_callback, this, std::placeholders::_1,
                                                      std::placeholders::_2));
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

    case inbound_packet::REGISTER: {
      std::cout << "Client registered on socket " << socket_src << std::endl;

      // Get all existing spreadsheets.
      auto spreadsheets = spreadsheet_controller_.get_spreadsheet_names();

      // Respond to the client.
      data_container_.new_outbound_packet(socket_src, *new outbound_connect_accepted_packet(spreadsheets));

      delete packet;

      break;
    }
    case inbound_packet::LOAD: {
      auto load_packet = dynamic_cast<inbound_load_packet *>(packet);
      std::cout << "Client on socket " << socket_src << " load on " << load_packet->get_spreadsheet_name() << std::endl;

      // Register the client with the given spreadsheet.
      data_container_.new_client(socket_src, load_packet->get_spreadsheet_name());

      // Drop into default to handle packet.
    }
    default: {
      data_container_.new_inbound_packet(*packet);

      break;
    }
  }
}
