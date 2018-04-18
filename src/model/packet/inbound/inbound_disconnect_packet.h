#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_DISCONNECT_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_DISCONNECT_PACKET_H


#include "inbound_packet.h"

/**
 * Represents a disconnection packet.
 */
class inbound_disconnect_packet : inbound_packet {
public:

    inbound_disconnect_packet(int socket_id, const std::string &raw_message);

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_DISCONNECT_PACKET_H
