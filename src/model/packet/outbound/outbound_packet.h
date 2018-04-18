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
  static const char EOT = '\3';

 public:

  /**
   * @return The raw message to be sent, including an EOT terminator (\3).
   */
  virtual std::string get_raw_message();

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_PACKET_H
