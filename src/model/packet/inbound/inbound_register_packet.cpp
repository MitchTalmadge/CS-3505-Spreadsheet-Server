#include "inbound_register_packet.h"

inbound_register_packet::inbound_register_packet(int socket_id_, const std::string &raw_contents_) : inbound_packet(socket_id_,
                                                                                                    raw_contents_) {}
