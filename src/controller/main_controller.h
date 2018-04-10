/*
main_controller.h

Main Controller:
Coordinate model/client activity, specifically:
- Create our network controller to handle communication from/with connected clients.
- Create models when network controller signals as such.
 */

#ifndef MAIN_CONTROLLER
#define MAIN_CONTROLLER

#include <vector>
#include <string>
#include "network_controller.h"
#include "data_container.h"

class main_controller
{

 private:
  // Data Container holds the data involved in communication between
  // the network controller and the models.
  data_container data;

  // Network Controller handles network interactions with clients.
  network_controller network_control;
  
  // Map from spreadsheet id to associated model.
  //  boost::unordered_map<std::string, spreadsheet> models;

 public:
  main_controller();

  // Handle a new client connecting.
  void handle_client(int socket_id);

  // Shutdown the controller and it's components.
  void shut_down();
};

#endif
