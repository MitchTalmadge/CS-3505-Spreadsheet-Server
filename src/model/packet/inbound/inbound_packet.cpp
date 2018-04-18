#include "inbound_packet.h"

inbound_packet::inbound_packet(int socket_id_, const std::string &raw_contents_) : socket_id_(socket_id_),
                                                                                   raw_contents_(raw_contents_) {}

int inbound_packet::get_socket_id_() const {
    return socket_id_;
}

const std::string &inbound_packet::get_raw_contents_() const {
    return raw_contents_;
}
