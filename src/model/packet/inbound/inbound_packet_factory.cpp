#include <boost/algorithm/string.hpp>
#include <controller/spreadsheet/spreadsheet_controller.h>
#include "inbound_packet_factory.h"
#include "inbound_register_packet.h"
#include "inbound_disconnect_packet.h"
#include "inbound_load_packet.h"
#include "inbound_ping_packet.h"
#include "inbound_ping_response_packet.h"
#include "inbound_undo_packet.h"
#include "inbound_edit_packet.h"
#include "inbound_revert_packet.h"
#include "inbound_unfocus_packet.h"
#include "inbound_focus_packet.h"

boost::optional<inbound_packet> inbound_packet_factory::from_raw_message(int socket_id,
                                                                         const std::string &raw_message) {

  // Check for empty message
  if (raw_message.empty())
    return boost::none;

  // Check for EOT terminator.
  if (!boost::ends_with(raw_message, EOT))
    return boost::none;

  // Check message types
  if (boost::starts_with(raw_message, "register ")) {
    return inbound_register_packet(socket_id, raw_message);
  } else if (boost::starts_with(raw_message, "disconnect ")) {
    return inbound_disconnect_packet(socket_id, raw_message);
  } else if (boost::starts_with(raw_message, "load ")) {
    // Extract spreadsheet name.
    return inbound_load_packet(socket_id, raw_message, raw_message.substr(5, raw_message.size() - 1 - 5));
  } else if (boost::starts_with(raw_message, "ping ")) {
    return inbound_ping_packet(socket_id, raw_message);
  } else if (boost::starts_with(raw_message, "ping_response ")) {
    return inbound_ping_response_packet(socket_id, raw_message);
  } else if (boost::starts_with(raw_message, "edit ")) {
    // Extract contents
    std::string contents = raw_message.substr(5, raw_message.size() - 1 - 5);

    // Split on :
    std::vector<std::string> split;
    boost::split(split, contents, boost::is_any_of(":"));

    // Make sure we have two parts.
    if (split.size() != 2)
      return boost::none;

    // Validate cell name.
    if (!spreadsheet_controller::is_valid_cell_name(split.front()))
      return boost::none;

    return inbound_edit_packet(socket_id, raw_message, split.front(), split.back());
  } else if (boost::starts_with(raw_message, "focus ")) {
    // Extract contents
    std::string contents = raw_message.substr(6, raw_message.size() - 1 - 6);

    // Validate cell name.
    if (!spreadsheet_controller::is_valid_cell_name(contents))
      return boost::none;

    return inbound_focus_packet(socket_id, raw_message, contents);
  } else if (boost::starts_with(raw_message, "unfocus ")) {
    // Extract contents
    std::string contents = raw_message.substr(8, raw_message.size() - 1 - 8);

    // Validate cell name.
    if (!spreadsheet_controller::is_valid_cell_name(contents))
      return boost::none;

    return inbound_unfocus_packet(socket_id, raw_message, contents);
  } else if (boost::starts_with(raw_message, "undo ")) {
    return inbound_undo_packet(socket_id, raw_message);
  } else if (boost::starts_with(raw_message, "revert ")) {
    // Extract contents
    std::string contents = raw_message.substr(7, raw_message.size() - 1 - 7);

    // Validate cell name.
    if (!spreadsheet_controller::is_valid_cell_name(contents))
      return boost::none;

    return inbound_revert_packet(socket_id, raw_message, contents);
  }

  return boost::none;
}
