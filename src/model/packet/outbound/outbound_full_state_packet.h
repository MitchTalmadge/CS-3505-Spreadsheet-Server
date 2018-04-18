#ifndef PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_FULL_STATE_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_FULL_STATE_PACKET_H

#include <map>
#include "outbound_packet.h"
class outbound_full_state_packet : outbound_packet {

  /**
   * Maps all non-empty cells from their names to their contents.
   */
  const std::map<std::string, std::string> cell_contents_;

 public:

  outbound_full_state_packet(const std::map<std::string, std::string> &cell_contents);

  outbound_packet_type get_packet_type() override;

  std::string get_raw_message() override;

  const std::map<std::string, std::string> &get_cell_contents() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_FULL_STATE_PACKET_H
