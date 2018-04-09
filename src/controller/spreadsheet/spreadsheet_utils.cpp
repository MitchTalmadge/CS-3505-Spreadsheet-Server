#include "spreadsheet_utils.h"
#include <regex>

bool is_valid_variable(const std::string &variable) {

    // Define variable regex pattern.
    static const std::regex pattern(R"(^[a-zA-Z_](?:[a-zA-Z_]|\d)*$)");

    // Compare variable to regex.
    std::smatch match;
    return std::regex_search(variable.begin(), variable.end(), match, pattern);
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
