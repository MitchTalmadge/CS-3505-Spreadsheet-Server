/*
data_container.h

Data container holds shared data between the network controller and the model.
This is meant to consolidate the areas where multiple threads will need to be 
able to access shared data.
 */

#ifndef PIGRAMMERS_SPREADSHEET_SERVER_DATA_CONTAINER
#define PIGRAMMERS_SPREADSHEET_SERVER_DATA_CONTAINER

#include <queue>
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <model/packet/inbound/inbound_packet.h>
#include <model/packet/outbound/outbound_packet.h>
#include <boost/optional/optional.hpp>

class data_container {

private:

    // Map from spreadsheet to associated sockets.
    std::map<std::string, std::vector<int> > spreadsheet_to_sockets;
    std::mutex spreadsheet_to_sockets_mutex;

    // Map from sockets to associated spreadsheet.
    std::map<int, std::string> sockets_to_spreadsheet;
    std::mutex sockets_to_spreadsheet_mutex;

    // Map from spreadsheet name to incoming message queue.
    std::map<std::string, std::queue<inbound_packet *> > inbound_messages;
    std::mutex inbound_messages_mutex;

    // Map from socket_id_ to outgoing message queue.
    std::map<int, std::queue<outbound_packet *> > outbound_messages;
    std::mutex outbound_messages_mutex;

    /**
     * Private constructor for singleton pattern.
     */
    data_container() = default;

    /**
     * Private destructor for singleton pattern.
     */
    ~data_container() = default;

public:

    /**
     * @return The singleton instance of this container.
     */
    static data_container &get_instance();

    /**
     * Deleted copy constructor since this is a singleton.
     */
    data_container(data_container const &) = delete;

    /**
     * Deleted assignment operator since this is a singleton.
     */
    void operator=(data_container const &)  = delete;

    // Get a message from the inbound queue for the given spreadsheet. Called by the model.
    inbound_packet * get_inbound_packet(std::string spreadsheet);

    // Send new message to inbound message.
    void new_inbound_packet(inbound_packet &packet);

    // Get outbound message from the given socket's queue.
    outbound_packet * get_outbound_packet(int socket_id);

    // Send new message to outbound queue for given socket.
    void new_outbound_packet(int socket_id, outbound_packet &packet);

    // Send new message to outbound queue for all sockets in the provided spreadsheet.
    void new_outbound_packet(std::string spreadsheet_name, outbound_packet &packet);
};

#endif
