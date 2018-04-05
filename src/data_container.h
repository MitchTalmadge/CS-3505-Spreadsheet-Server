/*
data_container.h

Data container holds shared data between the network controller and the model.
This is meant to consolidate the areas where multiple threads will need to be 
able to access shared data.
 */

#ifndef DATA_CONTAINER
#define DATA_CONTAINER

#include <queue>
#include <map>
#include <string>

class data_container
{

 private:

  // Map from spreadsheet name to incoming message queue.
  std::map<std::string, std::queue<std::string> > inbound_messages;

  // Map from socket_id to outgoing message queue.
  std::map<int, std::queue<std::string> > outbound_messages;

 public:
  data_container();
  
  // Get a message from the inbound queue for the given spreadsheet. Called by the model.
  std::string get_inbound_message(std::string spreadsheet);

  // Get outbound message from the given socket's queue.
  std::string get_outbound_message(int socket_id);
};

#endif
