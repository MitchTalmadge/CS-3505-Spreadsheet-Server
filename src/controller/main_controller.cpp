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
#include <model/packet/inbound_packet.h>

main_controller &main_controller::get_instance() {
    static main_controller instance;
    return instance;
}

/*
Clean up the main controller. Specifically, tell the network controller
and models to shut down.
 */
void main_controller::shut_down() {
    // TODO: Add shut down.
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
  // End of text character.
  char eot = '\3';

  // Parse the inbound packet.
  inbound_packet packet_in = parse(message);

    if (boost::regex_match(message, register_msg)) {
      // New client registering. Send it a connect_accepted message.
      std::cout << "Client registered on socket " << socket_src << std::endl;

      // Return the list of all available spreadsheets.
      std::string connect_accepted = spreadsheet_controller_.get_spreadsheets();

      std::cout << "Send string: " << connect_accepted << std::endl;
      
      // Add the message to its outgoing buffer.
      data_container_.new_outbound_message(socket_src, connect_accepted);
    } else if(boost::regex_match(message, load)) {
      // Determine the name of the spreadsheet the client would like to connect to.
      std::string spreadsheet_name = message.substr(5, message.length()-1);

      std::cout << "Client loading " << spreadsheet_name << std::endl;

      // TODO: Add more name checking? 
      
      std::string state_msg = spreadsheet_controller_.get_spreadsheet(spreadsheet_name);
      
      // Tell the data container that the given socket is associated with the given spreadsheet.
      data_container_.new_client(socket_src, spreadsheet_name);

      // Now add a message to the outbound queue of the socket to let the client know the state
      // of the spreadsheet.
      data_container_.new_outbound_message(socket_src, state_msg);
    } else {
      // For now, we simply drop the message we just received if it doesn't match.
    }
}
