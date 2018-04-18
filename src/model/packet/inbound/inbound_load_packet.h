#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_LOAD_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_LOAD_PACKET_H

#include "inbound_packet.h"

/**
 * Represents a client's request to load a spreadsheet.
 */
class inbound_load_packet : public inbound_packet {

  /**
   * The name of the spreadsheet to be loaded.
   */
  const std::string spreadsheet_name_;

 public:

  inbound_load_packet(int socket_id, const std::string &raw_message, const std::string &spreadsheet_name);

  inbound_packet_type get_packet_type() const override;

  const std::string &get_spreadsheet_name() const;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_LOAD_PACKET_H
