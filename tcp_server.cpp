/*
tcp_server.cpp

TCP Server listens for new connections and forwards them to the MainController
which handles setting up the network/model components for connected clients.
 */

#include "tcp_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

void tcp_server::startup()
{
  // TODO: Setup main_controller.

  // Start TCP server listening for connections:

  // AF_INET - use IPv4.
  // SOCK_STREAM - use TCP.
  // 0 is for protocol and again indicates IP.
  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    std::cout << "Socket failed to create." << std::endl;
    return;
  }

  // Create a sockaddr_in struct that determines the port and address of the server socket.
  sockaddr_in address;
  address.sin_family = AF_INET; // IPv4 address.
  address.sin_addr.s_addr = INADDR_ANY; // Use localhost
  address.sin_port = htons(this->port); // htons converts to network byte order.

  // Bind our server socket to the address/port specified above.
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cout << "Failed to bind socket to address." << std::endl;
  }

  // Set the server socket up to listen for incoming requests. Queue up to 5 connections.
  if (listen(server_fd, 5) < 0) {
    std::cout << "Failed to set socket to listen mode." << std::endl;
  }

  std::cout << "Set up socket to listen for new connections." << std::endl;
}

void tcp_server::server_work()
{
  // Nothing for now. Eventually move the actually listening for connections here.
}

tcp_server::tcp_server()
{
  // Set our port according to the communication protocol.
  this->port = 2112;
}

void tcp_server::shut_down()
{
  // Shutdown our socket.
  shutdown(this->server_fd, SHUT_RDWR);

  // TODO: tell main controller to shut down.
}
