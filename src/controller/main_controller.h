/*
main_controller.h

Main Controller:
Coordinate model/client activity, specifically:
- Create our network controller to handle communication from/with connected clients.
- Create models when network controller signals as such.
 */

#ifndef MAIN_CONTROLLER
#define MAIN_CONTROLLER

#include <vector>
#include <string>
#include <controller/network/network_controller.h>
#include "data_container.h"

class main_controller {

private:
    // Network Controller handles network interactions with clients.
    network_controller network_control;

    // Map from spreadsheet id to associated model.
    //  boost::unordered_map<std::string, spreadsheet> models;

    // List of all spreadsheets.
    std::vector<std::string> spreadsheets;

    // Callback from network controller to handle a message.
    std::string message_callback(int socket_src, std::string message);

public:
    // Handle a new client connecting.
    void handle_client(int socket_id);

    // Shutdown the controller and it's components.
    void shut_down();
};

#endif
