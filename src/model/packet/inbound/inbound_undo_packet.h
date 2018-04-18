#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNDO_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNDO_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's desire to undo the last change in the spreadsheet.
 */
class inbound_undo_packet : public inbound_packet {

 public:

  inbound_undo_packet(int socket_id, const std::string &raw_message);

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNDO_PACKET_H
