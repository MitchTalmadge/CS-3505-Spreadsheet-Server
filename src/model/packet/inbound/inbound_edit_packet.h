#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_EDIT_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_EDIT_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's edit to a cell.
 */
class inbound_edit_packet : public inbound_packet {

  /**
   * The name of the cell that is being edited.
   */
  const std::string cell_name_;

  /**
   * The new contents of the cell.
   */
  const std::string cell_contents_;

 public:

  inbound_edit_packet(int socket_id, const std::string &raw_message, const std::string &cell_name,
                      const std::string &cell_contents);

  const std::string &get_cell_name() const;

  const std::string &get_cell_contents() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_EDIT_PACKET_H
