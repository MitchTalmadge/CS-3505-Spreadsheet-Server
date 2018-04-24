#include "outbound_unfocus_packet.h"
outbound_unfocus_packet::outbound_unfocus_packet(const std::string &unique_id) : unique_id_(unique_id) {}

outbound_packet *outbound_unfocus_packet::clone() const {
  return new outbound_unfocus_packet(unique_id_);
}

outbound_packet::outbound_packet_type outbound_unfocus_packet::get_packet_type() {
  return UNFOCUS;
}

std::string outbound_unfocus_packet::get_raw_message() {
  return "unfocus " + unique_id_ + EOT;
}

const std::string &outbound_unfocus_packet::get_unique_id() const {
  return unique_id_;
}
