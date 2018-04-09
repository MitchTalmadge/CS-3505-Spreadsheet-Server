#include "spreadsheet_utils.h"
#include <algorithm>
#include <boost/regex>

bool is_valid_variable(const std::string &variable) {

    // Define variable regex pattern.
    static const boost::regex pattern(R"(^[a-zA-Z_](?:[a-zA-Z_]|\d)*$)");

    // Compare variable to regex.
    boost::smatch match;
    return boost::regex_search(variable.begin(), variable.end(), match, pattern);
}

bool is_double(const std::string &str) {
    try {
        // Attempt conversion
        std::stod(str);
        return true;
    } catch (...) {
        return false;
    }
}

std::string normalize_cell_name(std::string cellName) {
    std::transform(cellName.begin(), cellName.end(), cellName.begin(), ::toupper);
    return cellName;
}
