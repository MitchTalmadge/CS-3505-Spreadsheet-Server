#include "spreadsheet_controller.h"
#include <boost/regex.hpp>
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
#include <model/packet/outbound/outbound_connect_accepted_packet.h>
#include <model/packet/outbound/outbound_file_load_error_packet.h>

const std::string spreadsheet_controller::FILE_DIR_PATH = "saves";

spreadsheet_controller::spreadsheet_controller() {

  // Find all existing spreadsheets.
  boost::filesystem::create_directories(FILE_DIR_PATH);
  boost::filesystem::directory_iterator end;
  for (boost::filesystem::directory_iterator item(FILE_DIR_PATH); item != end; ++item) {
    available_spreadsheets_.insert(item->path().stem().string());
  }

  // Start work thread.
  worker_thread = new boost::thread(&spreadsheet_controller::work, this);
}

spreadsheet_controller::~spreadsheet_controller() {
  // Interrupt the worker thread.
  worker_thread->interrupt();
  delete worker_thread;

  // Save all spreadsheets.
  save_all_spreadsheets();

  // Delete all spreadsheets.
  for (auto &&item : active_spreadsheets_) {
    delete item.second;
  }
};

spreadsheet_controller &spreadsheet_controller::get_instance() {
  static spreadsheet_controller instance; // Instantiated on first-use.
  return instance;
}

void spreadsheet_controller::work() {

  while (true) {
    // Get an inbound packet.
    auto packet = data_container_.get_inbound_packet();
    if (packet)
      parse_inbound_packet(*packet);

    // Check if we should save.
    if (save_countdown_ <= 0) {
      // Reset counter.
      save_countdown_ = 18000;

      // Save all spreadsheets.
      save_all_spreadsheets();
    }

    try {
      // Briefly sleep to prevent this from choking machine resources.
      boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
    } catch (boost::thread_interrupted interruption) {
      // Thread interrupted.
      break;
    }
  }
}

void spreadsheet_controller::parse_inbound_packet(inbound_packet &packet) {

  // Attempt to find a related spreadsheet to this packet.
  auto sheet_pair = active_spreadsheets_.find(sockets_to_spreadsheets_[packet.get_socket_id()]);

  // Handle parsed packet.
  switch (packet.get_packet_type()) {
    case inbound_packet::REGISTER: {
      std::cout << "Registering client on socket ID " << packet.get_socket_id() << std::endl;

      // Register the socket.
      data_container_.register_socket(packet.get_socket_id());

      // Respond to the client.
      data_container_.new_outbound_packet(packet.get_socket_id(),
                                          *new outbound_connect_accepted_packet(available_spreadsheets_));

      break;
    }
    case inbound_packet::EDIT: {
      auto edit_packet = dynamic_cast<inbound_edit_packet &>(packet);

      // Attempt to assign contents
      sheet_pair->second->set_cell_contents(edit_packet.get_cell_name(), edit_packet.get_cell_contents());

      // Relay change to all clients
      send_packet_to_all_sockets(sheet_pair->first, *new outbound_change_packet(edit_packet.get_cell_name(),
                                                                                edit_packet.get_cell_contents()));
      break;
    }
    case inbound_packet::LOAD: {
      auto load_packet = dynamic_cast<inbound_load_packet &>(packet);

      // Try to find an existing spreadsheet.
      auto item = available_spreadsheets_.find(load_packet.get_spreadsheet_name());
      if (item != available_spreadsheets_.end()) {
        // Spreadsheet is available for loading. Check if it has already been loaded.
        auto item2 = active_spreadsheets_.find(load_packet.get_spreadsheet_name());
        if (item2 != active_spreadsheets_.end()) {
          // Spreadsheet is already loaded.
          std::cout << "Loading spreadsheet from memory: " + load_packet.get_spreadsheet_name() << std::endl;
          data_container_.new_outbound_packet(packet.get_socket_id(),
                                              *new outbound_full_state_packet(item2->second->get_non_empty_cells()));
        } else {
          // Spreadsheet must be loaded from file.
          std::cout << "Loading spreadsheet from file: " + load_packet.get_spreadsheet_name() << std::endl;

          auto sheet = new spreadsheet(FILE_DIR_PATH + "/" + load_packet.get_spreadsheet_name() + ".sprd");
          if (sheet->is_loaded()) {
            std::cout << "Load was successful." << std::endl;
            active_spreadsheets_[load_packet.get_spreadsheet_name()] = sheet;
            data_container_.new_outbound_packet(packet.get_socket_id(),
                                                *new outbound_full_state_packet(sheet->get_non_empty_cells()));
          } else {
            std::cout << "Load failed." << std::endl;
            delete sheet;
            data_container_.new_outbound_packet(packet.get_socket_id(),
                                                *new outbound_file_load_error_packet());
          }
        }
      } else {
        // Create new spreadsheet.
        std::cout << "Creating new spreadsheet: " + load_packet.get_spreadsheet_name() << std::endl;

        active_spreadsheets_[load_packet.get_spreadsheet_name()] = new spreadsheet;
        available_spreadsheets_.insert(load_packet.get_spreadsheet_name());

        // Save the new spreadsheet.
        save_spreadsheet(*active_spreadsheets_[load_packet.get_spreadsheet_name()], load_packet.get_spreadsheet_name());

        data_container_.new_outbound_packet(packet.get_socket_id(),
                                            *new outbound_full_state_packet(active_spreadsheets_[load_packet.get_spreadsheet_name()]->get_non_empty_cells()));
      }

      // Assign socket to spreadsheet and vice-versa
      sockets_to_spreadsheets_[packet.get_socket_id()] = load_packet.get_spreadsheet_name();
      spreadsheets_to_sockets_[load_packet.get_spreadsheet_name()].insert(packet.get_socket_id());

      break;
    }
    case inbound_packet::FOCUS: {
      auto focus_packet = dynamic_cast<inbound_focus_packet &>(packet);

      // Focus on the cell.
      sheet_pair->second->focus_cell(focus_packet.get_socket_id(), focus_packet.get_cell_name());

      send_packet_to_all_sockets(sheet_pair->first,
                                 *new outbound_focus_packet(focus_packet.get_cell_name(),
                                                            std::to_string(focus_packet.get_socket_id())));
      break;
    }
    case inbound_packet::UNFOCUS: {
      // Unfocus the cell.
      sheet_pair->second->unfocus_cell(packet.get_socket_id());

      send_packet_to_all_sockets(sheet_pair->first,
                                 *new outbound_unfocus_packet(std::to_string(packet.get_socket_id())));
      break;
    }
    case inbound_packet::UNDO: {
      // Undo the cell changes
      auto result = sheet_pair->second->undo();

      // Check if undo had any effect.
      if (result)
        send_packet_to_all_sockets(sheet_pair->first,
                                   *new outbound_change_packet(result.get().first, result.get().second));
      break;
    }
    case inbound_packet::REVERT: {
      auto revert_packet = dynamic_cast<inbound_revert_packet &>(packet);

      // Revert the changes on the cell.
      auto result = sheet_pair->second->revert(revert_packet.get_cell_name());

      // Check if revert had any effect.
      if (result)
        send_packet_to_all_sockets(sheet_pair->first,
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

void spreadsheet_controller::send_packet_to_all_sockets(const std::string &spreadsheet_name,
                                                        outbound_packet &packet) {

  // Send to all sockets mapped to the given spreadsheet.
  auto iter = spreadsheets_to_sockets_.find(spreadsheet_name);
  if (iter != spreadsheets_to_sockets_.end()) {

    std::vector<int> to_remove;

    for (auto socket_id : iter->second) {
      // Clone the packet into each socket's outbound queue.
      bool still_connected = data_container_.new_outbound_packet(socket_id, *packet.clone());

      if (!still_connected) {
        to_remove.push_back(socket_id);
      }
    }

    // Remove disconnected sockets.
    for (auto socket_id : to_remove) {
      iter->second.erase(socket_id);
    }
  }

  // Dispose of the original packet.
  delete &packet;
}

void spreadsheet_controller::save_all_spreadsheets() const {
  std::cout << "Saving All Spreadsheets..." << std::endl;
  for (auto &&item : active_spreadsheets_) {
    save_spreadsheet(*item.second, item.first);
  }
}

void spreadsheet_controller::save_spreadsheet(spreadsheet &sheet, const std::string &spreadsheet_name) const {
  std::cout << "Saving Spreadsheet: " << spreadsheet_name << std::endl;
  sheet.save_to_file(FILE_DIR_PATH + "/" + spreadsheet_name + ".sprd");
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