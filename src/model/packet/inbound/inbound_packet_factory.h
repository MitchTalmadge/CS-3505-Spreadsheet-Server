#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_FACTORY_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_FACTORY_H

#include "inbound_packet.h"
#include <boost/optional/optional.hpp>

/**
 * Creates inbound packets.
 */
class inbound_packet_factory {

 private:

  /**
   * The End of Text terminator.
   */
  static const std::string EOT;

 public:

  /**
   * Parses the given message into a packet if possible.
   * @param socket_id The ID of the socket that sent the packet.
   * @param raw_message The raw, un-modified contents of the packet including an EOT terminator (\3).
   * @return An inbound packet that was parsed from the message. If the message could not be parsed, boost::none will be returned.
   */
  static inbound_packet *from_raw_message(int socket_id, const std::string &raw_message);

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_PACKET_FACTORY_H
