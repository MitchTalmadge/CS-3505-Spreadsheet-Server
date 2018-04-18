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
#include <model/packet/inbound/inbound_packet.h>
#include <model/packet/inbound/inbound_packet_factory.h>
#include <model/packet/inbound/inbound_register_packet.h>
#include <model/packet/inbound/inbound_load_packet.h>

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

    boost::optional<inbound_packet> packet_optional = inbound_packet_factory::from_raw_message(socket_src, message);

    // Check if packet was parsed.
    if (!packet_optional)
        return;

    // Handle parsed packet.
    if (auto register_packet = dynamic_cast<inbound_register_packet &>(packet_optional.get())) {
        // New client registering. Send it a connect_accepted message.
        std::cout << "Client registered on socket " << socket_src << std::endl;

        // Return the list of all available spreadsheets.
        std::string connect_accepted = spreadsheet_controller_.get_spreadsheets();

        std::cout << "Send string: " << connect_accepted << std::endl;

        // Add the message to its outgoing buffer.
        data_container_.new_outbound_packet(socket_src, connect_accepted);
    } else {
        data_container_.new_inbound_packet(packet_optional.get());
    }
}
