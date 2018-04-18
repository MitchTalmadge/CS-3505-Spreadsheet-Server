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


void data_container::new_inbound_packet(inbound_packet packet) {

}

/*
Allow the model to get a message for the given string spreadsheet.
 */
boost::optional<inbound_packet> data_container::get_inbound_packet(std::string spreadsheet) {
    return boost::none;
}

/*
Send a new message to the specified socket.
Called by the main controller during registration.
 */
void data_container::new_outbound_packet(int socket_id, outbound_packet packet) {

    outbound_messages_mutex.lock();

    auto queue = outbound_messages[socket_id];
    queue.push(packet);

    outbound_messages_mutex.unlock();
}

/*
Send a new message to all the attached sockets for the given spreadsheet.
Called by the spreadsheet models.
 */
void data_container::new_outbound_packet(std::string spreadsheet, outbound_packet packet) {
    // TODO
}

/*
Allow for socket to grab an outbound message to be sent to client.
 */
boost::optional<outbound_packet> data_container::get_outbound_packet(int socket_id) {
    outbound_messages_mutex.lock();

    auto queue = outbound_messages[socket_id];

    if (queue.empty()) {
        outbound_messages_mutex.unlock();
        return boost::none;
    }

    auto packet = queue.front();
    queue.pop();

    outbound_messages_mutex.unlock();
    return packet;
}
