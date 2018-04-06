/*
tcp_server.h

TCP Server listens for new connections and forwards them to the MainController
which handles setting up the network/model components for connected clients.
 */

#ifndef PIGRAMMERS_SPREADSHEET_SERVER_TCP_SERVER
#define PIGRAMMERS_SPREADSHEET_SERVER_TCP_SERVER

#include <boost/thread/thread.hpp>

class tcp_server {
private:
    // Main controller that handles the network/model components for connected clients.
    //main_controller main_control;

    // TCP Port to listen on.
    int port;

    // Server socket file descriptor. Identifies our TCP server socket.
    int server_fd;

    // Thread to do all the listening work.
    boost::thread worker_thread;

    // Work loop for our server - listen for new clients.
    void server_work();

public:
    tcp_server();

    // Start up our TCP Server.
    void startup();

    // Shut down the server and it's components.
    void shut_down();
};

#endif
