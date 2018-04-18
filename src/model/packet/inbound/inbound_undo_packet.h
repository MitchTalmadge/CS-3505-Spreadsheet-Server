#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNDO_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNDO_PACKET_H


#include "inbound_packet.h"

class inbound_undo_packet : inbound_packet {
public:
    inbound_undo_packet(int socket_id, const std::string &raw_message);

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_UNDO_PACKET_H
