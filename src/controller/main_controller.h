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
#include "data_container.h"

class main_controller {

    // Network Controller handles network interactions with clients.
    network_controller &network_controller_ = network_controller::get_instance();

    // Map from spreadsheet id to associated model.
    //  boost::unordered_map<std::string, spreadsheet> models;

    // List of all spreadsheets.
    std::vector<std::string> spreadsheets;

    // Callback from network controller to handle a message.
    std::string message_callback(int socket_src, std::string message);

    /**
     * Private constructor for singleton pattern.
     */
    main_controller() = default;

    /**
     * Private destructor for singleton pattern.
     */
    ~main_controller() = default;

public:

    /**
     * @return The singleton instance of this controller.
     */
    static main_controller &get_instance();

    /**
     * Deleted copy constructor since this is a singleton.
     */
    main_controller(main_controller const &) = delete;

    /**
     * Deleted assignment operator since this is a singleton.
     */
    void operator=(main_controller const &) = delete;

    // Handle a new client connecting.
    void handle_client(int socket_id);

    // Shutdown the controller and it's components.
    void shut_down();
};

#endif
