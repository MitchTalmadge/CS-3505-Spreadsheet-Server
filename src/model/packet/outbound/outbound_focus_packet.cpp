#include "outbound_focus_packet.h"
outbound_focus_packet::outbound_focus_packet(const std::string &cell_name, const std::string &unique_id) : cell_name_(
    cell_name), unique_id_(unique_id) {}

outbound_packet *outbound_focus_packet::clone() const {
  return new outbound_focus_packet(cell_name_, unique_id_);
}

outbound_packet::outbound_packet_type outbound_focus_packet::get_packet_type() {
  return FOCUS;
}

std::string outbound_focus_packet::get_raw_message() {
  return "focus " + cell_name_ + ":" + unique_id_ + EOT;
}

const std::string &outbound_focus_packet::get_cell_name() const {
  return cell_name_;
}

const std::string &outbound_focus_packet::get_unique_id() const {
  return unique_id_;
}

