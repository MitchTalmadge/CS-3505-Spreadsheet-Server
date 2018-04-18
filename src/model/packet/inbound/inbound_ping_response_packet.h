
#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PING_RESPONSE_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PING_RESPONSE_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's response to a ping packet sent from the server to the client.
 */
class inbound_ping_response_packet : inbound_packet {

 public:

  inbound_ping_response_packet(int socket_id, const std::string &raw_message);

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PING_RESPONSE_PACKET_H
