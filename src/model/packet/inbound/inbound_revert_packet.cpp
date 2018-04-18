#include "inbound_revert_packet.h"

inbound_revert_packet::inbound_revert_packet(int socket_id, const std::string &raw_message,
                                             const std::string &cell_name) : inbound_packet(socket_id, raw_message),
                                                                             cell_name_(cell_name) {}

const std::string &inbound_revert_packet::get_cell_name() const {
  return cell_name_;
}
