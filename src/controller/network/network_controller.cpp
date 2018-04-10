/*
network_controller.cpp

Network Controller handles maintaining connections and communication with connected
clients.
 */

#include "network_controller.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/ref.hpp>

/*
Create a new network controller. Set up any variables as necessary.
 */
network_controller::network_controller(data_container& data_container)
{
  data = data_container;
}

/*
Work loop for the network controller, where it listens in on the provided socket 
and checks to see if it needs to send a message out on the given socket.
 */
void network_controller::socket_work_loop(int socket_id)
{
  std::cout << "Listening on socket " << socket_id << std::endl;

  char buffer[1024] = {0};
  
  while(true)
  {
    read(socket_id, &buffer, 1024);
    
    std::cout << "Message from client: " << std::endl;
    std::cout << buffer << std::endl;

    // memset(buffer, 0, 1024);

    // TODO: Handle outbound message queues to.
  }
}

/*
Perform the handshake as specified by the protocol with the given socket to determine
the spreadsheet this client would like to work on.
 */
std::string network_controller::handshake(int socket_id)
{
  // TODO: perform actual handshake.

  return "";
}

/*
When a new client connects, determine whether to create a new queue for a new
spreadsheet, then setup a loop listening on the new socket for communications.
 */
void network_controller::start_work(int socket_id)
{
  // TODO: determine if we have a queue for the given spreadsheet already.

  // Start a thread that can listen in on the given socket and handle communications.
  boost::thread new_thread(&network_controller::socket_work_loop, this, socket_id);
}

/*
Clean up and shut down.
 */
void network_controller::shut_down()
{
  // TODO: shutdown each socket?
}
