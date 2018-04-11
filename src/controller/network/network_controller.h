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
#include <functional>
#include <controller/data_container.h>

class network_controller
{

 private:
  // Data container is where outgoing messages will be stored.
  data_container data;

  // Work loop for the network controller, where it listens in on a socket for incoming
  // messages.
  void socket_work_loop(int socket_id, std::function<std::string (int, std::string)> callback);

 public:
  network_controller(data_container& data);

  // Create a work loop for the provided socket.
  void start_work(int socket_id, std::function<std::string (int, std::string)> callback);

  // Shut down self and components.
  void shut_down();
};

#endif
