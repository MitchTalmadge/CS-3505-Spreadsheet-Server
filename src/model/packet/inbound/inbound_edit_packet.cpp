#include "inbound_edit_packet.h"

inbound_edit_packet::inbound_edit_packet(int socket_id, const std::string &raw_message,
                                         const std::string &cell_name, const std::string &cell_contents)
    : inbound_packet(socket_id, raw_message), cell_name_(cell_name), cell_contents_(cell_contents) {}

inbound_packet::inbound_packet_type inbound_edit_packet::get_packet_type() const {
  return EDIT;
}

const std::string &inbound_edit_packet::get_cell_name() const {
  return cell_name_;
}

const std::string &inbound_edit_packet::get_cell_contents() const {
  return cell_contents_;
}