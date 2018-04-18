#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_H

#include <string>

/**
 * Represents a single packet received by the server from a client.
 */
class inbound_packet {

 protected:

  /**
   * The ID of the socket that sent the packet.
   */
  const int socket_id_;

  /**
   * The raw, un-modified message that composes this packet. Includes an EOT terminator (\3).
   */
  const std::string raw_message_;

  inbound_packet(int socket_id, const std::string &raw_message);

 public:

  int get_socket_id() const;

  const std::string &get_raw_message() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_H
