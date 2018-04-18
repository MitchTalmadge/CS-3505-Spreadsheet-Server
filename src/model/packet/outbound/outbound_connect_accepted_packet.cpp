#include <boost/algorithm/string.hpp>
#include "outbound_connect_accepted_packet.h"

outbound_connect_accepted_packet::outbound_connect_accepted_packet(const std::vector<std::string> &spreadsheet_names)
    : spreadsheet_names_(spreadsheet_names) {}

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

const std::vector<std::string> &outbound_connect_accepted_packet::get_spreadsheet_names() const {
  return spreadsheet_names_;
}