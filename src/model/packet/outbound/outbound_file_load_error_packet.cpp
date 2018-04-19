#include "outbound_file_load_error_packet.h"

outbound_packet::outbound_packet_type outbound_file_load_error_packet::get_packet_type() {
  return FILE_LOAD_ERROR;
}
std::string outbound_file_load_error_packet::get_raw_message() {
  return "file_load_error " + EOT;
}
