#include <boost/algorithm/string.hpp>
#include "outbound_full_state_packet.h"
outbound_full_state_packet::outbound_full_state_packet(const std::map<std::string, std::string> &cell_contents)
    : cell_contents_(cell_contents) {}
outbound_packet::outbound_packet_type outbound_full_state_packet::get_packet_type() {
  return FULL_STATE;
}
std::string outbound_full_state_packet::get_raw_message() {
  std::string response = "full_state ";

  // Append all cell contents.
  for (auto &&item : cell_contents_) {
    response + item.first + ":" + item.second + "\n";
  }

  // Remove last newline.
  if (boost::ends_with(response, "\n"))
    response.erase(response.size() - 1, 1);

  return response + EOT;
}
const std::map<std::string, std::string> &outbound_full_state_packet::get_cell_contents() const {
  return cell_contents_;
}
