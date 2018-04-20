#ifndef PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_UNFOCUS_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_UNFOCUS_PACKET_H

#include "outbound_packet.h"
class outbound_unfocus_packet : public outbound_packet {

  /**
   * A unique ID associated with the user who is focusing on the cell.
   */
  std::string unique_id_;

 public:

  explicit outbound_unfocus_packet(const std::string &unique_id);

  outbound_packet *clone() const override;

  outbound_packet_type get_packet_type() override;

  std::string get_raw_message() override;

  const std::string &get_unique_id() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_UNFOCUS_PACKET_H
