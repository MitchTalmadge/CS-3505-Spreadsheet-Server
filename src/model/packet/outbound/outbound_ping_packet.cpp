#include "outbound_ping_packet.h"
outbound_packet::outbound_packet_type outbound_ping_packet::get_packet_type() {
  return PING;
}
std::string outbound_ping_packet::get_raw_message() {
  return "ping " + EOT;
}
