#include "outbound_change_packet.h"

outbound_change_packet::outbound_change_packet(const std::string &cell_name, const std::string &cell_contents)
    : cell_name_(cell_name), cell_contents_(cell_contents) {}

outbound_packet *outbound_change_packet::clone() const {
  return new outbound_change_packet(cell_name_, cell_contents_);
}

outbound_packet::outbound_packet_type outbound_change_packet::get_packet_type() {
  return CHANGE;
}

std::string outbound_change_packet::get_raw_message() {
  return "change " + cell_name_ + ":" + cell_contents_ + EOT;
}

const std::string &outbound_change_packet::get_cell_name() const {
  return cell_name_;
}

const std::string &outbound_change_packet::get_cell_contents() const {
  return cell_contents_;
}
