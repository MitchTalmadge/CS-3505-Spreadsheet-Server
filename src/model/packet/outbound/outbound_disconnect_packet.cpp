#include "outbound_disconnect_packet.h"
outbound_packet *outbound_disconnect_packet::clone() const {
  return new outbound_disconnect_packet();
}

outbound_packet::outbound_packet_type outbound_disconnect_packet::get_packet_type() {
  return DISCONNECT;
}
std::string outbound_disconnect_packet::get_raw_message() {
  return "disconnect " + EOT;
}
