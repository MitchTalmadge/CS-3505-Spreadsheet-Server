#include "spreadsheet_controller.h"
#include <algorithm>
#include <boost/regex.hpp>

bool spreadsheet_controller::is_valid_cell_name(const std::string &cell_name) {

    // Define cell name regex pattern.
    static const boost::regex pattern(R"(^[a-zA-Z_](?:[a-zA-Z_]|\d)*$)");

    // Compare cell name to regex.
    boost::smatch match;
    return boost::regex_search(cell_name.begin(), cell_name.end(), match, pattern);
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