#include <boost/algorithm/string.hpp>
#include "outbound_connect_accepted_packet.h"

outbound_connect_accepted_packet::outbound_connect_accepted_packet(const std::set<const std::string> &spreadsheet_names)
    : spreadsheet_names_(spreadsheet_names) {}

outbound_packet *outbound_connect_accepted_packet::clone() const {
  return new outbound_connect_accepted_packet(spreadsheet_names_);
}

outbound_packet::outbound_packet_type outbound_connect_accepted_packet::get_packet_type() {
  return CONNECT_ACCEPTED;
}

std::string outbound_connect_accepted_packet::get_raw_message() {
  std::string response = "connect_accepted ";

  // Append all spreadsheet names.
  for (auto spreadsheet_name : spreadsheet_names_) {
    response += spreadsheet_name + "\n";
  }

  // Remove last newline.
  if (boost::ends_with(response, "\n"))
    response.erase(response.size() - 1, 1);

  return response + EOT;
}

const std::set<const std::string> &outbound_connect_accepted_packet::get_spreadsheet_names() const {
  return spreadsheet_names_;
}
