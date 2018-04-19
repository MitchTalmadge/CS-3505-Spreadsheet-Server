#include "inbound_load_packet.h"

inbound_load_packet::inbound_load_packet(int socket_id, const std::string &raw_message,
                                         const std::string &spreadsheet_name) : inbound_packet(socket_id,
                                                                                               raw_message),
                                                                                spreadsheet_name_(spreadsheet_name) {}

inbound_packet::inbound_packet_type inbound_load_packet::get_packet_type() const {
  return LOAD;
}

const std::string &inbound_load_packet::get_spreadsheet_name() const {
  return spreadsheet_name_;
}
