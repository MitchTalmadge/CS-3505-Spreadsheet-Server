#include "inbound_packet.h"
#include <boost/algorithm/string/predicate.hpp>

inbound_packet::inbound_packet(int socket_id, const std::string &raw_message) : socket_id_(socket_id),
                                                                                raw_message_(raw_message) {}

int inbound_packet::get_socket_id() const {
  return socket_id_;
}

const std::string &inbound_packet::get_raw_message() const {
  return raw_message_;
}
