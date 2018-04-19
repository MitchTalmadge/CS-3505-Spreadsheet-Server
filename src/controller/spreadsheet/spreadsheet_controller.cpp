#include "spreadsheet_controller.h"
#include <boost/regex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <model/packet/inbound/inbound_edit_packet.h>
#include <model/packet/outbound/outbound_change_packet.h>
#include <model/packet/outbound/outbound_full_state_packet.h>
#include <model/packet/inbound/inbound_focus_packet.h>
#include <model/packet/outbound/outbound_focus_packet.h>
#include <model/packet/outbound/outbound_unfocus_packet.h>
#include <boost/filesystem.hpp>
#include <model/packet/inbound/inbound_load_packet.h>
#include <model/packet/inbound/inbound_revert_packet.h>
#include <iostream>

const std::string spreadsheet_controller::FILE_DIR_PATH = "saves";

spreadsheet_controller::spreadsheet_controller() {

  // Load all existing spreadsheets.
  boost::filesystem::directory_iterator end;
  for (boost::filesystem::directory_iterator item(FILE_DIR_PATH); item != end; ++item) {
    spreadsheet *sheet = new spreadsheet(item->path().string());
    if (sheet->is_loaded()) {
      active_spreadsheets_[item->path().stem().string()] = sheet;
    } else {
      delete sheet;
    }
  }

  // Start work thread.
  boost::thread work_thread(&spreadsheet_controller::work, this);
}

spreadsheet_controller::~spreadsheet_controller() {
  // Save all spreadsheets.
  get_instance().save_all_spreadsheets();

  // Delete all spreadsheets.
  for (auto &&item : active_spreadsheets_) {
    delete item.second;
  }
};

spreadsheet_controller &spreadsheet_controller::get_instance() {
  static spreadsheet_controller instance; // Instantiated on first-use.
  return instance;
}

void spreadsheet_controller::shut_down() {
  delete &get_instance();
}

void spreadsheet_controller::work() {

  // Iterate over all active spreadsheets
  for (auto &entry : active_spreadsheets_) {
    // Check for inbound messages.
    auto packet = data_container_.get_inbound_packet(entry.first);
    if (packet) {
      // Parse inbound message.
      parse_inbound_packet(*packet, entry.first, *entry.second);
    }
  }

  // Check if we should save.
  if (save_countdown_ <= 0) {
    // Reset counter.
    save_countdown_ = 18000;

    // Save all spreadsheets.
    save_all_spreadsheets();
  }

  // Briefly sleep to prevent this from choking machine resources.
  boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
}

void spreadsheet_controller::parse_inbound_packet(inbound_packet &packet, const std::string &spreadsheet_name,
                                                  spreadsheet &sheet) {

  // Handle parsed packet.
  switch (packet.get_packet_type()) {
    case inbound_packet::EDIT: {
      auto edit_packet = dynamic_cast<inbound_edit_packet &>(packet);

      // Attempt to assign contents
      sheet.set_cell_contents(edit_packet.get_cell_name(), edit_packet.get_cell_contents());

      // Relay change to all clients
      data_container_.new_outbound_packet(spreadsheet_name,
                                          *new outbound_change_packet(edit_packet.get_cell_name(),
                                                                      edit_packet.get_cell_contents()));
      break;
    }
    case inbound_packet::LOAD: {
      auto load_packet = dynamic_cast<inbound_load_packet &>(packet);

      // Try to find an existing spreadsheet.
      auto item = active_spreadsheets_.find(load_packet.get_spreadsheet_name());
      if (item != active_spreadsheets_.end()) {
        // Spreadsheet found.
        std::cout << "Loading existing spreadsheet: " + item->first << std::endl;
        data_container_.new_outbound_packet(packet.get_socket_id(),
                                            *new outbound_full_state_packet(item->second->get_non_empty_cells()));
      } else {
        // Create new spreadsheet.
        std::cout << "Creating new spreadsheet: " + load_packet.get_spreadsheet_name() << std::endl;
        active_spreadsheets_[load_packet.get_spreadsheet_name()] = new spreadsheet;
        data_container_.new_outbound_packet(packet.get_socket_id(),
                                            *new outbound_full_state_packet(active_spreadsheets_[load_packet.get_spreadsheet_name()]->get_non_empty_cells()));
      }

      break;
    }
    case inbound_packet::FOCUS: {
      auto focus_packet = dynamic_cast<inbound_focus_packet &>(packet);
      sheet.focus_cell(focus_packet.get_socket_id(), focus_packet.get_cell_name());

      data_container_.new_outbound_packet(spreadsheet_name,
                                          *new outbound_focus_packet(focus_packet.get_cell_name(),
                                                                     std::to_string(focus_packet.get_socket_id())));
      break;
    }
    case inbound_packet::UNFOCUS: {
      sheet.unfocus_cell(packet.get_socket_id());

      data_container_.new_outbound_packet(spreadsheet_name,
                                          *new outbound_unfocus_packet(std::to_string(packet.get_socket_id())));
      break;
    }
    case inbound_packet::UNDO: {
      auto result = sheet.undo();

      // Check if undo had any effect.
      if (result)
        data_container_.new_outbound_packet(spreadsheet_name,
                                            *new outbound_change_packet(result.get().first, result.get().second));
      break;
    }
    case inbound_packet::REVERT: {
      auto revert_packet = dynamic_cast<inbound_revert_packet &>(packet);
      auto result = sheet.revert(revert_packet.get_cell_name());

      // Check if revert had any effect.
      if (result)
        data_container_.new_outbound_packet(spreadsheet_name,
                                            *new outbound_change_packet(result.get().first, result.get().second));
      break;
    }
    default: {
      break;
    }
  }

  // Dispose of packet.
  delete &packet;
}

void spreadsheet_controller::save_all_spreadsheets() const {
  for (auto &&item : active_spreadsheets_) {
    item.second->save_to_file(FILE_DIR_PATH + "/" + item.first + ".sprd");
  }
}

bool spreadsheet_controller::is_valid_cell_name(const std::string &cell_name) {

  // Define cell name regex pattern.
  static const boost::regex pattern(R"(^[A-Z][1-9][0-9]?$)");

  // Compare normalized cell name to regex.
  const std::string normalized_cell_name = spreadsheet_controller::normalize_cell_name(cell_name);
  boost::smatch match;
  return boost::regex_search(normalized_cell_name.begin(), normalized_cell_name.end(), match, pattern);
}

std::string spreadsheet_controller::normalize_cell_name(std::string cellName) {
  std::transform(cellName.begin(), cellName.end(), cellName.begin(), ::toupper);
  return cellName;
}

std::vector<std::string> spreadsheet_controller::get_spreadsheet_names() const {
  std::vector<std::string> names;

  for (auto &&item : active_spreadsheets_) {
    names.push_back(item.first);
  }

  return names;
}