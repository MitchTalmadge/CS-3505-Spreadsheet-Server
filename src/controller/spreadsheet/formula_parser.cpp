#include "formula_parser.h"
#include "spreadsheet_utils.h"
#include <algorithm>
#include <regex>

std::vector<std::string> formula_parser::extract_tokens(const std::string &formula) {
    // Create vector for returning.
    std::vector<std::string> tokens;

    // Define search pattern
    static const std::regex pattern(
            R"((\())" // Left Parenthesis
            R"(|(\)))" // Right Parenthesis
            R"(|([\+\-*/]))" // Operator
            R"(|([a-zA-Z_](?:[a-zA-Z_]|\d)*))" // Variable
            R"(|((?:\d+\.\d*|\d*\.\d+|\d+)(?:[eE][\+-]?\d+)?))" // Double
    );

    // Split formula with pattern
    std::sregex_token_iterator iterator(formula.begin(), formula.end(), pattern, -1);
    std::sregex_token_iterator end;
    for (; iterator != end; iterator++)
        tokens.push_back(*iterator);

    return tokens;
}

bool formula_parser::is_valid(std::string formula) {
    // Remove all spaces.
    std::remove(formula.begin(), formula.end(), ' ');

    // Empty formulas are invalid.
    if (formula.empty())
        return false;

    // Extract tokens from formula
    auto tokens = extract_tokens(formula);

    // Check the first token for syntax errors.
    auto firstToken = tokens.front();
    if (firstToken != "(" && !is_double(firstToken) && !is_valid_variable(firstToken)) {
        return false;
    }

    // Check the last token for syntax errors.
    auto lastToken = tokens.back();
    if(lastToken != ")" && !is_double(lastToken) && !is_valid_variable(lastToken)) {
        return false;
    }

}

std::set<std::string> formula_parser::find_dependents(const std::string &formula) {
    // Invalid formulas always return an empty set.
    if (!is_valid(formula))
        return std::set<std::string>();

    return std::set<std::string>();
}
