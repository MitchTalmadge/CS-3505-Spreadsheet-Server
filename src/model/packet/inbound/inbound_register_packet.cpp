#include "inbound_register_packet.h"

inbound_register_packet::inbound_register_packet(int socket_id, const std::string &raw_message) : inbound_packet(
    socket_id,
    raw_message) {}

inbound_packet::inbound_packet_type inbound_register_packet::get_packet_type() const {
  return REGISTER;
}
