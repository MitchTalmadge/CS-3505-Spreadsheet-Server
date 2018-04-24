#ifndef PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_FOCUS_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_FOCUS_PACKET_H

#include "outbound_packet.h"
class outbound_focus_packet : public outbound_packet {

  /**
   * The name of the cell to focus on.
   */
  std::string cell_name_;

  /**
   * A unique ID associated with the user who is focusing on the cell.
   */
  std::string unique_id_;

 public:

  outbound_focus_packet(const std::string &cell_name, const std::string &unique_id);

  outbound_packet *clone() const override;

  outbound_packet_type get_packet_type() override;

  std::string get_raw_message() override;

  const std::string &get_cell_name() const;

  const std::string &get_unique_id() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_FOCUS_PACKET_H
