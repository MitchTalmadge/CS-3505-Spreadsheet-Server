#include "inbound_edit_packet.h"

inbound_edit_packet::inbound_edit_packet(int socket_id_, const std::string &raw_contents_,
                                         const std::string &cell_name_, const std::string &cell_contents_)
        : inbound_packet(socket_id_, raw_contents_), cell_name_(cell_name_), cell_contents_(cell_contents_) {}

const std::string &inbound_edit_packet::get_cell_name_() const {
    return cell_name_;
}

const std::string &inbound_edit_packet::get_cell_contents_() const {
    return cell_contents_;
}
