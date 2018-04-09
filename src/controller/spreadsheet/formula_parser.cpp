#include "formula_parser.h"
#include "spreadsheet_utils.h"
#include <algorithm>
#include <boost/regex.hpp>

std::vector<std::string> formula_parser::extract_tokens(const std::string &formula) {
    // Create vector for returning.
    std::vector<std::string> tokens;

    // Define search pattern
    static const boost::regex pattern(
            R"((\())" // Left Parenthesis
            R"(|(\)))" // Right Parenthesis
            R"(|([\+\-*/]))" // Operator
            R"(|([a-zA-Z_](?:[a-zA-Z_]|\d)*))" // Cell name
            R"(|((?:\d+\.\d*|\d*\.\d+|\d+)(?:[eE][\+-]?\d+)?))" // Double
            R"(|[^\s])" // All others, except spaces
    );

    // Split formula with pattern
    boost::sregex_token_iterator iterator(formula.begin(), formula.end(), pattern);
    boost::sregex_token_iterator end;
    for (; iterator != end; iterator++)
        tokens.push_back(*iterator);

    return tokens;
}

std::pair<bool, std::set<std::string> > formula_parser::parse_formula(std::string formula) {
    // Empty formula
    if (formula.empty())
        return std::pair<bool, std::set<std::string> >(false, {});

    // Extract tokens from formula
    auto tokens = extract_tokens(formula);

    // Empty formula
    if (tokens.size() == 0)
        return std::pair<bool, std::set<std::string> >(false, {});

    // Check the first token for syntax errors.
    auto firstToken = tokens.front();
    if (firstToken != "(" && !is_double(firstToken) && !is_valid_cell_name(firstToken)) {
        return std::pair<bool, std::set<std::string> >(false, {});
    }

    // Check the last token for syntax errors.
    auto lastToken = tokens.back();
    if (lastToken != ")" && !is_double(lastToken) && !is_valid_cell_name(lastToken)) {
        return std::pair<bool, std::set<std::string> >(false, {});
    }

    // The number of left and right parentheses encountered, respectively.
    int parentheses_count[2] = {0, 0};

    // The dependents of this formula.
    std::set<std::string> dependents;

    // Iterate over each token
    std::string previous_token;
    for (const auto &token : tokens) {

        if (!previous_token.empty()) {
            // Ensure that any token immediately following an opening parenthesis or operator
            // is either another opening parenthesis, a number, or a cell name.
            if (previous_token == "(" || previous_token == "+" || previous_token == "-" || previous_token == "*" ||
                previous_token == "/") {
                if (token != "(" && !is_double(token) && !is_valid_cell_name(token))
                    return std::pair<bool, std::set<std::string> >(false, {});
            }

            // Ensure that any token immediately following a closing parenthesis, number, or cell name
            // is either another closing parenthesis or an operator.
            if (previous_token == ")" || is_double(previous_token) || is_valid_cell_name(previous_token)) {
                if (token != ")" && token != "+" && token != "-" && token != "*" &&
                    token != "/") {
                    return std::pair<bool, std::set<std::string> >(false, {});
                }
            }
        }

        // Count parentheses.
        if (token == "(")
            parentheses_count[0]++;
        else if (token == ")") {
            parentheses_count[1]++;
        } else if (is_valid_cell_name(token)) {
            dependents.insert(normalize_cell_name(token));
        }

        previous_token = token;
    }

    // Ensure parentheses are balanced.
    if (parentheses_count[0] != parentheses_count[1])
        return std::pair<bool, std::set<std::string> >(false, {});

    return std::pair<bool, std::set<std::string> >(true, dependents);
}

bool formula_parser::is_valid(const std::string &formula) {
    return parse_formula(formula).first;
}

std::set<std::string> formula_parser::find_dependents(const std::string &formula) {
    return parse_formula(formula).second;
}