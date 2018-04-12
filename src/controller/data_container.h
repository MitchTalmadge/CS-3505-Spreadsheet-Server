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

class data_container {

private:

    // Map from spreadsheet to associated sockets.
    std::map<std::string, std::vector<int> > spreadsheet_to_sockets;

    // Map from sockets to associated spreadsheet.
    std::map<int, std::string> sockets_to_spreadsheet;

    // Map from spreadsheet name to incoming message queue.
    std::map<std::string, std::queue<std::string> > inbound_messages;

    // Map from socket_id to outgoing message queue.
    std::map<int, std::queue<std::string> > outbound_messages;

public:

    data_container();

    // Insert a new client into the mapping from sockets to spreadsheets and vice versa.
    void new_client(int socket_id, std::string spreadsheet);

    // Get a message from the inbound queue for the given spreadsheet. Called by the model.
    std::string get_inbound_message(std::string spreadsheet);

    // Send new message to inbound message.
    void new_inbound_message(int socket_src, std::string message);

    // Get outbound message from the given socket's queue.
    std::string get_outbound_message(int socket_id);

    // Send new message to outbound message.
    void new_outbound_message(std::string spreadsheet_src, std::string message);
};

#endif
