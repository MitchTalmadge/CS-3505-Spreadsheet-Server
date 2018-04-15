/*
data_container.cpp

Data container holds shared data between the network controller and the model.
This is meant to consolidate the areas where multiple threads will need to be 
able to access shared data.
 */

#include "data_container.h"
#include <iostream>

/*
Create a new data container.
 */
data_container::data_container() {
    // Nothing for now.
}

/*
Insert the new container into the mapping from spreadsheets to sockets and vice versa.
 */
void data_container::new_client(int socket_id, std::string spreadsheet) {
  // Client queue should be made during the registration process.
}

void data_container::new_inbound_message(int socket_id, std::string message) {
    // TODO
}

/*
Allow the model to get a message for the given string spreadsheet.
 */
std::string data_container::get_inbound_message(std::string spreadsheet) {
  // TODO

    return "";
}

/*
Send a new message to the specified socket.
Called by the main controller during registration.
 */
void data_container::new_outbound_message(int socket_id, std::string message) {

  outbound_messages_mutex.lock();
  // First, check if a queue already exists for this socket.
  auto it = outbound_messages.find(socket_id);
  if (it != outbound_messages.end())
  {  
    // Add the message to the queue.
    it->second.push(message);
  } else {
    // Create a new queue and insert the message.
    std::queue<std::string> new_queue;
    new_queue.push(message);
    // Put it in our map.
    outbound_messages[socket_id] = new_queue;
  }
  outbound_messages_mutex.unlock();
}

/*
Send a new message to all the attached sockets for the given spreadsheet.
Called by the spreadsheet models.
 */
void data_container::new_outbound_message(std::string spreadsheet, std::string message) {
  // TODO
}

/*
Allow for socket to grab an outbound message to be sent to client.
 */
std::string data_container::get_outbound_message(int socket_id) {
  outbound_messages_mutex.lock();
  auto it = outbound_messages.find(socket_id);

  if (it != outbound_messages.end()) {
    if (!outbound_messages[socket_id].empty()) {
      std::cout << "Grabbing message." << std::endl;

      std::string msg = outbound_messages[socket_id].front();
      outbound_messages[socket_id].pop();

      std::cout << "Sending message." << std::endl;

      outbound_messages_mutex.unlock();
      return msg;
    } else {
      outbound_messages_mutex.unlock();
      return "";
    }
  } else {
    outbound_messages_mutex.unlock();
    return "";
  }
}
