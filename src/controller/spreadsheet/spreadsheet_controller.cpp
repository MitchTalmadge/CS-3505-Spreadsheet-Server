#include "spreadsheet_controller.h"
#include <algorithm>
#include <boost/regex.hpp>

bool spreadsheet_controller::is_valid_cell_name(const std::string &cell_name) {

    // Define cell name regex pattern.
    static const boost::regex pattern(R"(^[A-Z][1-99]$)");

    // Compare normalized cell name to regex.
    auto normalized_cell_name = normalized_cell_name(cell_name);
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