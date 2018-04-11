/*
network_controller.cpp

Network Controller handles maintaining connections and communication with connected
clients.
 */

#include "network_controller.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <boost/ref.hpp>
#include <boost/regex.hpp>
#include <boost/chrono.hpp>
#include <string.h>

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
void network_controller::socket_work_loop(int socket_id, std::function<std::string(int, std::string)> callback)
{
  std::cout << "Listening on socket " << socket_id << std::endl;

  char buffer[1024] = {0};
  
  while(true)
  {
    // Wait for a message to arrive.
    read(socket_id, &buffer, 1024);
    
    std::cout << "Message from client: " << std::endl;
    std::cout << buffer << std::endl;

    // TODO: Handle incomplete messages - move this to main_controller?

    // Send messages to the main_controller who decides where to send them next.
    callback(socket_id, buffer);

    // TODO: Read from outbound message queue.
    std::string message = data.get_outbound_message(socket_id);

    if (message != "")
    {
      const char * msg = message.c_str();
      write(socket_id, msg, sizeof(msg));
    }

    boost::this_thread::sleep_for(boost::chrono::seconds{4});
  }
}

void set_socket_non_blocking(int socket_id)
{
  // TODO: error handling.

  int flags = fcntl(socket_id, F_GETFL, 0);
  flags = flags | O_NONBLOCK;
  fcntl(socket_id, F_SETFL, flags);
}

/*
When a new client connects, determine whether to create a new queue for a new
spreadsheet, then setup a loop listening on the new socket for communications.
 */
void network_controller::start_work(int socket_id, std::function<std::string (int, std::string)> callback)
{
  set_socket_non_blocking(socket_id);

  // Start a thread that can listen in on the given socket.
  boost::thread work_thread(&network_controller::socket_work_loop, this, socket_id, callback);
}

/*
Clean up and shut down.
 */
void network_controller::shut_down()
{
  // TODO: shutdown each socket?
}
