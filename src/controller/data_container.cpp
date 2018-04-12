/*
data_container.cpp

Data container holds shared data between the network controller and the model.
This is meant to consolidate the areas where multiple threads will need to be 
able to access shared data.
 */

#include "data_container.h"

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

void data_container::new_outbound_message(std::string spreadsheet, std::string message) {
    // TODO
}

/*
Allow for socket to grab an outbound message to be sent to client.
 */
std::string data_container::get_outbound_message(int id) {
    // TODO
    return "";
}
