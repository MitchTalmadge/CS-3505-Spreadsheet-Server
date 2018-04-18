#include "inbound_disconnect_packet.h"

inbound_disconnect_packet::inbound_disconnect_packet(int socket_id_, const std::string &raw_contents_) : inbound_packet(socket_id_,
                                                                                                        raw_contents_) {
}
