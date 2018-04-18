#ifndef PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_DISCONNECT_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_DISCONNECT_PACKET_H

#include "outbound_packet.h"
class outbound_disconnect_packet : public outbound_packet  {

 public:
  outbound_packet_type get_packet_type() override;
  std::string get_raw_message() override;

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_OUTBOUND_DISCONNECT_PACKET_H
