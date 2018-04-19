#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_DISCONNECT_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_DISCONNECT_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client disconnecting from the server.
 */
class inbound_disconnect_packet : public inbound_packet {
 public:

  inbound_disconnect_packet(int socket_id, const std::string &raw_message);

  inbound_packet_type get_packet_type() const override;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_DISCONNECT_PACKET_H
