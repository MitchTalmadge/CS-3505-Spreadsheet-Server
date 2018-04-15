/*
network_controller.h

Network Controller handles maintaining connections and communication with connected
clients.
 */

#ifndef PIGRAMMERS_SPREADSHEET_SERVER_NETWORK_CONTROLLER
#define PIGRAMMERS_SPREADSHEET_SERVER_NETWORK_CONTROLLER

#include <map>
#include <vector>
#include <string>
#include <boost/thread/thread.hpp>
#include <functional>
#include <controller/data_container.h>

class network_controller {

private:
    // Data container is where outgoing messages will be stored.
    data_container &data = data_container::get_instance();

    // Work loop for the network controller, where it listens in on a socket for incoming
    // messages.
    void socket_work_loop(int socket_id, std::function<std::string(int, std::string)> callback);

    /**
     * Private constructor for singleton pattern.
     */
    network_controller() = default;

    /**
     * Private destructor for singleton pattern.
     */
    ~network_controller() = default;

public:

    /**
     * @return The singleton instance of this controller.
     */
    static network_controller &get_instance();

    /**
     * Deleted copy constructor since this is a singleton.
     */
    network_controller(network_controller const &) = delete;

    /**
     * Deleted assignment operator since this is a singleton.
     */
    void operator=(network_controller const &)  = delete;

    // Create a work loop for the provided socket.
    void start_work(int socket_id, std::function<std::string(int, std::string)> callback);

    // Shut down self and components.
    void shut_down();
};

#endif
