#include "spreadsheet_controller.h"
#include <algorithm>
#include <boost/regex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string.hpp>

spreadsheet_controller::spreadsheet_controller() {

    // Start work thread.
    boost::thread work_thread(spreadsheet_controller::work);
}

spreadsheet_controller::~spreadsheet_controller() = default;

spreadsheet_controller &spreadsheet_controller::get_instance() {
    static spreadsheet_controller instance; // Instantiated on first-use.
    return instance;
}

void spreadsheet_controller::work() {

    // Iterate over all active spreadsheets
    for (auto iterator = active_spreadsheets_.begin(); iterator != active_spreadsheets_.end(); iterator++) {
        // Check for inbound messages.
        std::string message = data_container_.get_inbound_message(iterator->first);
        if (!message.empty()) {
            // Parse inbound message.
            parse_inbound_message(message, iterator->first, iterator->second);
        }
    }

    // Briefly sleep to prevent this from choking machine resources.
    boost::this_thread::sleep_for(boost::chrono::milliseconds{10});
}

void spreadsheet_controller::parse_inbound_message(const std::string &message, const std::string &spreadsheet_name,
                                                   spreadsheet &sheet) {
    static std::string edit_prefix = "edit";

    // Check if this is an edit message. Example: "edit A1:=2*3+A2\3"
    if (message.compare(0, edit_prefix.size(), edit_prefix)) {
        // Extract contents of message, excluding "edit " and "\3"
        std::string edit_contents = message.substr(edit_prefix.size() + 1, message.size() - 1);

        // Split on :
        std::vector<std::string> split;
        boost::split(split, edit_contents, boost::is_any_of(":"));

        // Verify that the message was formatted correctly.
        if (split.size() != 2)
            return;

        // Attempt to assign contents
        sheet.set_cell_contents(split.front(), split.back());

        // Relay change to all clients
        data_container_.new_outbound_message(spreadsheet_name,
                                             "change " + split.front() + ":" + split.back() + ((char) 3));
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