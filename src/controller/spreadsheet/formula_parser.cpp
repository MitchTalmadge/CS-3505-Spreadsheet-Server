#include "formula_parser.h"
#include <algorithm>

bool formula_parser::is_valid(std::string formula) {
    if(formula == nullptr)
        return false;

    // Remove all spaces.
    std::replace(formula.begin(), formula.end(), ' ', '');

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
