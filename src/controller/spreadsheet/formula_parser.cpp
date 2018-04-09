#include "formula_parser.h"
#include <algorithm>

bool formula_parser::is_valid(std::string formula) {
    // Remove all spaces.
    std::remove(formula.begin(), formula.end(), ' ');

    // Empty formulas are invalid.
    if(formula.empty())
        return false;
}

std::set<std::string> formula_parser::find_dependents(const std::string &formula) {
    // Invalid formulas always return an empty set.
    if(!is_valid(formula))
        return std::set<std::string>();

    return std::set<std::string>();
}
