#include "inbound_unfocus_packet.h"

inbound_unfocus_packet::inbound_unfocus_packet(int socket_id, const std::string &raw_message) : inbound_packet(socket_id, raw_message) {}

inbound_packet::inbound_packet_type inbound_unfocus_packet::get_packet_type() const {
  return UNFOCUS;
}