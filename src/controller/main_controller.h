/*
main_controller.h

Main Controller:
Coordinate model/client activity, specifically:
- Create our network controller to handle communication from/with connected clients.
- Create models when network controller signals as such.
 */

#ifndef PIGRAMMERS_SPREADSHEET_SERVER_MAIN_CONTROLLER
#define PIGRAMMERS_SPREADSHEET_SERVER_MAIN_CONTROLLER

#include <vector>
#include <string>
#include <controller/network/network_controller.h>

class main_controller {

private:
    // Network Controller handles network interactions with clients.
    network_controller network_control;

    // Vector of active model ids.
    std::vector <std::string> active_models;
    // Vector of actual active models.
    //std::vector<spreadsheet> spreadsheets;

    // Handle a new client connecting.
    void handle_client(std::string spreadsheet, int socket_id);

    // Shutdown the controller and it's components.
    void shut_down();

public:
    main_controller();
};

#endif
