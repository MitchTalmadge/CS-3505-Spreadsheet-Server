#include "inbound_revert_packet.h"

inbound_revert_packet::inbound_revert_packet(int socket_id, const std::string &raw_message,
                                             const std::string &cell_name_) : inbound_packet(socket_id, raw_message),
                                                                              cell_name(cell_name_) {}

const std::string &inbound_revert_packet::get_cell_name() const {
    return cell_name;
}
