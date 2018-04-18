#include "inbound_disconnect_packet.h"

inbound_disconnect_packet::inbound_disconnect_packet(int socket_id, const std::string &raw_message) : inbound_packet(
    socket_id,
    raw_message) {
}
