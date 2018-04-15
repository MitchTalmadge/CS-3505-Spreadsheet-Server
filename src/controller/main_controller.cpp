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

/*
Create a new main controller.
 */
main_controller::main_controller() :
        network_control(data) {
    // Nothing for now.
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
    network_control.start_work(socket_id, std::bind(&main_controller::message_callback, this, std::placeholders::_1,
                                                    std::placeholders::_2));
}

/*
Handle a message arriving from a client. Specifically:
- If a register, queue an outgoing message with list of spreadsheets.
- If a load, determine if new model necessary.
- Handle disconnects. (How?)
- Forward edits into the corresponding spreadsheets inbound queue.
 */
std::string main_controller::message_callback(int socket_src, std::string message) {
    // End of text character.
    char eot = (char) 3;

    boost::regex register_msg{ "register " + eot};
    boost::regex disconnect{"disconnect " + eot };
    boost::regex load{"load .+" + eot };
    boost::regex ping{"ping " + eot };
    boost::regex ping_response{"ping_response " + eot };
    boost::regex edit{"edit [a-zA-Z][1-9][0-9]?:.*" + eot };
    boost::regex focus{"focus [a-zA-Z][1-9][0-9]?" + eot };
    boost::regex unfocus{"unfocus [a-zA-Z][1-9][0-9]?" + eot };
    boost::regex undo{ "undo " + eot };
    boost::regex revert{ "revert [a-zA-Z][1-9][0-9]?" + eot };

    if (boost::regex_match(message, register_msg)) {
      // New client registering. Send it a connect_accepted message.
      std::cout << "Client registered on socket " << socket_src << std::endl;

      // TODO: Add list of spreadsheets to this message.
      std::string connect_accepted = "connect_accepted " + eot;
      
      // Add the message to its outgoing buffer.
      data.new_outbound_message(socket_src, connect_accepted);
    } else if(boost::regex_match(message, load)) {
      // Determine the name of the spreadsheet the client would like to connect to.
      std::string sprdsheet_name = message.substr(5, message.length()-1);

      // TODO: Add more name checking?
      
      // TODO: Check if a model exists.
      /*
      if (sprdsheet_name in existing_models)
        get and send spreadsheet contents.
      else
        */
      // Associate the user with the given spreadsheet.
      data.new_client(socket_src, sprdsheet_name);

      std::string empty_sprdsheet = "full_state " + eot;
      data.new_outbound_message(socket_src, empty_sprdsheet);

      // TODO: Alert model of client, if required.
    } else {
      // For now, we simply drop the message we just received.
    }

    return "";
}
