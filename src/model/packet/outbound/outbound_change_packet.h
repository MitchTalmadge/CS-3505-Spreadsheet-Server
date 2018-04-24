#ifndef PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_CHANGE_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_CHANGE_PACKET_H

#include "outbound_packet.h"

class outbound_change_packet : public outbound_packet {

  /**
   * The name of the changed cell.
   */
  const std::string cell_name_;

  /**
   * The new contents of the changed cell.
   */
  const std::string cell_contents_;

 public:

  outbound_change_packet(const std::string &cell_name, const std::string &cell_contents);

  outbound_packet *clone() const override;

  outbound_packet_type get_packet_type() override;

  std::string get_raw_message() override;

  const std::string &get_cell_name() const;

  const std::string &get_cell_contents() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_CHANGE_PACKET_H
