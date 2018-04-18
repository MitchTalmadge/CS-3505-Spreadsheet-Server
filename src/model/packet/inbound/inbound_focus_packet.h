#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_FOCUS_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_FOCUS_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's intent to edit a cell.
 */
class inbound_focus_packet : inbound_packet {

  /**
   * The cell being focused upon.
   */
  const std::string cell_name_;

 public:

  inbound_focus_packet(int socket_id, const std::string &raw_message, const std::string &cell_name);

  const std::string &get_cell_name() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_FOCUS_PACKET_H
