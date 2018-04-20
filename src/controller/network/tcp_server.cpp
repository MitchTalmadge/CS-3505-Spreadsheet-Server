/*
tcp_server.cpp

TCP Server listens for new connections and forwards them to the MainController
which handles setting up the network/model components for connected clients.
 */

#include "tcp_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <boost/chrono.hpp>

/*
Create a new TCP Server. Here we set the port number and can handle any other setup required.
 */
tcp_server::tcp_server() {
  // Set our port according to the communication protocol.
  this->port = 2112;

  this->worker_thread = nullptr;
}

/*
Startup has two main roles:
1. Setup our Main Controller which handles coordination between model and active network connections.
2. Setup the listening socket that listens for new connections.
 */
bool tcp_server::startup() {
  // Start TCP server listening for connections:

  // AF_INET - use IPv4.
  // SOCK_STREAM - use TCP.
  // 0 is for protocol and again indicates IP.
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    std::cout << "Socket failed to create." << std::endl;
    return false;
  }

  // Create a sockaddr_in struct that determines the port and address of the server socket.
  sockaddr_in address;
  address.sin_family = AF_INET; // IPv4 address.
  address.sin_addr.s_addr = INADDR_ANY; // Use localhost
  address.sin_port = htons(this->port); // htons converts to network byte order.

  // Set some socket options so it reuses the port.
  int opt = 1;
  int res = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  if (res < 0) {
    std::cout << "Failed to set socket settings correctly." << std::endl;
    return false;
  }

  // Bind our server socket to the address/port specified above.
  res = bind(server_fd, (struct sockaddr *) &address, sizeof(address));
  if (res < 0) {
    std::cout << "Failed to bind socket to address." << std::endl;
    return false;
  }

  // Set the server socket up to listen for incoming requests. Queue up to 5 connections.
  if (listen(server_fd, 5) < 0) {
    std::cout << "Failed to set socket to listen mode." << std::endl;
    return false;
  }

  // Make the listening socket non-blocking to allow for clean shut down.
  if (!network_controller::set_socket_non_blocking(server_fd)) {
    std::cout << "Failed to set listening socket to non-blocking." << std::endl;
    return false;
  }

  std::cout << "Set up socket to listen for new connections." << std::endl;

  // Start a thread to handle continuously checking for new connections.
  worker_thread = new boost::thread(&tcp_server::server_work,
                                this); // this is the implicit first argument for any member function.

  return true;
}

/*
This is the work loop the tcp_server object will run continuously. It basically keeps checking
for new connections and forwards them on as necessary.
 */
void tcp_server::server_work() {
  // Check for new connections to the listening socket.
  while (true) {
    // Grab a connection from the top of the server listener queue.
    int new_socket = accept(server_fd, NULL, NULL);

    // For now just announce the new connection.
    if (new_socket != -1) {
      std::cout << "New connection received." << std::endl;
      main_controller_.handle_client(new_socket);
    }

    try {
      // Briefly sleep to prevent this from choking machine resources.
      boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
    } catch (boost::thread_interrupted interruption) {
      // Thread interrupted.
      break;
    }
  }
}

/*
Shut down the tcp_server, that is, the listening socket. Then forward the shutdown to the main
controller.
 */
tcp_server::~tcp_server() {
  // Interrupt the server work loop - this will interrupt next time it sleeps.
  if (worker_thread != nullptr) {
    worker_thread->interrupt();
    delete worker_thread;
  }

  // Shutdown our socket.
  close(server_fd);
}


