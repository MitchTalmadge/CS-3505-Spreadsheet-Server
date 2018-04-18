#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_REGISTER_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_REGISTER_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a registration packet.
 */
class inbound_register_packet : inbound_packet {
public:

    inbound_register_packet(int socket_id, const std::string &raw_message);

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_REGISTER_PACKET_H
