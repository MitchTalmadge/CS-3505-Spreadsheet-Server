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
When a new client is connected for the given spreadsheet, forward it to our network controller
who will control the interactions with it.
 */
void main_controller::handle_client(int socket_id) {
  // Forward the new socket to the network controller who sets up a communication loop.
  network_controller_.start_work(socket_id);
}
