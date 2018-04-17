#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_H

#include <string>

/**
 * Represents a single packet received by the server from a client.
 */
class inbound_packet {

    /**
     * The ID of the socket that sent the packet.
     */
    int socket_id_;

    /**
     * The raw, un-modified contents of the packet including an EOT terminator. (\3)
     */
    std::string raw_contents_;

public:

    inbound_packet(int socket_id_, const std::string &raw_contents_);

    int get_socket_id_() const;

    const std::string &get_raw_contents_() const;

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_H
