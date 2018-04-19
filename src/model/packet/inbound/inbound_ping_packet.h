#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PING_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PING_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a ping sent from client to server.
 */
class inbound_ping_packet : public inbound_packet {

 public:

  inbound_ping_packet(int socket_id, const std::string &raw_message);

  inbound_packet_type get_packet_type() const override;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PING_PACKET_H
