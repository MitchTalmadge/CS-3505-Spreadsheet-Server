#ifndef PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_PACKET_H

#include <string>

/**
 * Represents a packet sent from the server to one or more clients.
 */
class outbound_packet {

 protected:

  /**
    * The end of text terminator, to be appended to all raw messages.
    */
  static const std::string EOT;

  outbound_packet() = default;

 public:

  /**
   * Clones this packet.
   * @return The new, cloned packet.
   */
  virtual outbound_packet * clone() const = 0;

  /**
   * Represents the type of the packet, to avoid costly casting checks.
   */
  enum outbound_packet_type {
    CONNECT_ACCEPTED,
    FILE_LOAD_ERROR,
    DISCONNECT,
    PING,
    PING_RESPONSE,
    FULL_STATE,
    CHANGE,
    FOCUS,
    UNFOCUS
  };

  /**
   * @return The type of the packet.
   */
  virtual outbound_packet_type get_packet_type() = 0;

  /**
   * @return The raw message to be sent, including an EOT terminator (\3).
   */
  virtual std::string get_raw_message() = 0;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_PACKET_H
