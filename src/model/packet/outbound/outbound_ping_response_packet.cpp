#include "outbound_ping_response_packet.h"
outbound_packet::outbound_packet_type outbound_ping_response_packet::get_packet_type() {
  return PING_RESPONSE;
}
std::string outbound_ping_response_packet::get_raw_message() {
  return "ping_response " + EOT;
}
