/*
network_controller.h

Network Controller handles maintaining connections and communication with connected
clients.
 */

#ifndef NETWORK_CONTROLLER
#define NETWORK_CONTROLLER

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
  void socket_work_loop(int socket_id, std::string spreadsheet);

 public:
  network_controller(data_container& data);

  // Handle network communication for a newly connected socket.
  void handle_new_client(int socket_id, std::string spreadsheet);

  // Shut down self and components.
  void shut_down();
};

#endif
