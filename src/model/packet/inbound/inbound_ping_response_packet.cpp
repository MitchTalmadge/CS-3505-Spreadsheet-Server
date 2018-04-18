#include "inbound_ping_response_packet.h"

inbound_ping_response_packet::inbound_ping_response_packet(int socket_id, const std::string &raw_message)
    : inbound_packet(socket_id, raw_message) {}

inbound_packet::inbound_packet_type inbound_ping_response_packet::get_packet_type() const {
  return PING_RESPONSE
}
