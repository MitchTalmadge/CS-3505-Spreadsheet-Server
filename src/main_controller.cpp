/*
main_controller.cpp

Main Controller:
Coordinate model/client activity, specifically:
- Create our network controller to handle communication from/with connected clients.
- Create models when network controller signals as such.
 */

#include "main_controller.h"

/*
Create a new main controller.
 */
main_controller::main_controller():
  network_control(data)
{
  // Nothing for now.
}

/*
Clean up the main controller. Specifically, tell the network controller
and models to shut down.
 */
void main_controller::shut_down()
{
  // TODO: Add shut down.
}

/*
When a new client is connected for the given spreadsheet, determine whether
or not to spawn a new model thread for a new spreadsheet and forward the socket
to the network controller so that we can get communication.
 */
void main_controller::handle_client(int socket_id)
{
  // Determine which spreadsheet the new client wants to edit.
  std::string spreadsheet = network_control.handshake(socket_id);

  // Tell the data container the new mapping.
  data.new_client(socket_id, spreadsheet);

  // TODO: Determine if we need to spawn a new model.

  // Forward the new socket to the network controller who sets up a communication loop.
  network_control.start_work(socket_id);
}
