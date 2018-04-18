#include "inbound_unfocus_packet.h"

inbound_unfocus_packet::inbound_unfocus_packet(int socket_id, const std::string &raw_message,
                                               const std::string &cell_name) : inbound_packet(socket_id, raw_message),
                                                                               cell_name_(cell_name) {}

inbound_packet::inbound_packet_type inbound_unfocus_packet::get_packet_type() const {
  return UNFOCUS;
}

const std::string &inbound_unfocus_packet::get_cell_name() const {
  return cell_name_;
}
