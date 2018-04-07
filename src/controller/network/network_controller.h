/*
network_controller.h

Network Controller handles maintaining connections and communication with connected
clients.
 */

#ifndef PIGRAMMERS_SPREADSHEET_SERVER_NETWORK_CONTROLLER
#define PIGRAMMERS_SPREADSHEET_SERVER_NETWORK_CONTROLLER

#include <unordered_map>
#include <vector>
#include <string>
#include <queue>

class network_controller {

private:
    // Queue messages for a given spreadsheet.
    std::unordered_map<std::string, std::queue<std::string>> in_messages;

    // Convert from socket id to corresponding spreadsheet.
    std::unordered_map<int, std::string> socket_spreadsheet;

public:
    network_controller();

    // Handle network communication for a newly connected socket.
    void handle_new_client(int socket_id, std::string spreadsheet);

    // Get a message for a given spreadsheet.
    std::string get_message(std::string spreadsheet);

    // Send a message to the given socket.
    void send_message(int socket_id, std::string message);

    // Shut down self and components.
    void shut_down();
};

#endif