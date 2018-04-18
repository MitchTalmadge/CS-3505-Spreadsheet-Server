#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_REVERT_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_REVERT_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's desire to revert a cell.
 */
class inbound_revert_packet : public inbound_packet {

  /**
   * The name of the cell to revert.
   */
  const std::string cell_name_;

 public:

  inbound_revert_packet(int socket_id, const std::string &raw_message, const std::string &cell_name);

  inbound_packet_type get_packet_type() const override;

  const std::string &get_cell_name() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_REVERT_PACKET_H
