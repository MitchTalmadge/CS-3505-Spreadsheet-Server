#include "inbound_undo_packet.h"

inbound_undo_packet::inbound_undo_packet(int socket_id, const std::string &raw_message) : inbound_packet(socket_id,
                                                                                                         raw_message) {}

inbound_packet::inbound_packet_type inbound_undo_packet::get_packet_type() const {
  return UNDO;
}
