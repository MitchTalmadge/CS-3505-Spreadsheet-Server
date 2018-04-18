#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNFOCUS_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNFOCUS_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's intent to no longer focus upon a previously focused cell.
 */
class inbound_unfocus_packet : public inbound_packet {

 public:

  inbound_unfocus_packet(int socket_id, const std::string &raw_message);

  inbound_packet_type get_packet_type() const override;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNFOCUS_PACKET_H
