/*
network_controller.h

Network Controller handles maintaining connections and communication with connected
clients.
 */

#ifndef NETWORK_CONTROLLER
#define NETWORK_CONTROLLER

#include <map>
#include "network_controller.h"
#include <vector>
#include <string>
#include <boost/thread/thread.hpp>
#include "data_container.h"

class network_controller
{

 private:
  // Data container is where messages should be written to and where outgoing
  // messages are read from.
  data_container data;

  // Vector containing all threads being used to handle socket communciations.
  std::vector<boost::thread> active_threads;

  // Work loop for the network controller, where it listens in on a socket for incoming
  // messages and listens for new sockets to send from the model.
  void socket_work_loop(int socket_id);

 public:
  network_controller(data_container& data);

  // Create a work loop for the provided socket.
  void start_work(int socket_id);

  // Perform protocol handshake to determine what this client wants to edit in.
  std::string handshake(int socket_id);

  // Shut down self and components.
  void shut_down();
};

#endif
