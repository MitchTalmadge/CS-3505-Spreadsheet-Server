#include "spreadsheet_controller.h"
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <model/packet/inbound/inbound_edit_packet.h>
#include <model/packet/inbound/inbound_load_packet.h>

spreadsheet_controller::spreadsheet_controller() {

    // Start work thread.
    boost::thread work_thread(&spreadsheet_controller::work, this);
}

spreadsheet_controller::~spreadsheet_controller() = default;

spreadsheet_controller &spreadsheet_controller::get_instance() {
    static spreadsheet_controller instance; // Instantiated on first-use.
    return instance;
}

void spreadsheet_controller::work() {

    // Iterate over all active spreadsheets
    for (auto &entry : active_spreadsheets_) {
        // Check for inbound messages.
        auto packet_optional = data_container_.get_inbound_packet(entry.first);
        if (packet_optional) {
            // Parse inbound message.
            parse_inbound_packet(packet_optional.get(), entry.first, *(entry.second));
        }
    }

    // Briefly sleep to prevent this from choking machine resources.
    boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
}

void spreadsheet_controller::parse_inbound_packet(inbound_packet packet, const std::string &spreadsheet_name,
                                                  spreadsheet &sheet) {
    if(auto edit_packet = dynamic_cast<inbound_edit_packet &>(packet)) {
        // Attempt to assign contents
        sheet.set_cell_contents(edit_packet.get_cell_name(), edit_packet.get_cell_contents());

        // Relay change to all clients
        data_container_.new_outbound_packet(spreadsheet_name,
                                            "change " + split.front() + ":" + split.back() + ((char) 3));
    } else if (auto load_packet = dynamic_cast<inbound_load_packet &>(packet)) {
        // full_state
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


bool spreadsheet_controller::is_double(const std::string &str) {
    try {
        // Attempt conversion
        std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}


std::string spreadsheet_controller::get_spreadsheets() {
  // TODO: Update this with actual spreadsheet list.
  char eot = '\3';

  std::string msg = "connected_accepted ";
  return msg + eot;
}