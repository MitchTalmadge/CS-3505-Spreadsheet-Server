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

std::pair<bool, std::set<std::string> > formula_parser::parse_formula(std::string formula) {
    // Remove all spaces.
    std::remove(formula.begin(), formula.end(), ' ');

    // Empty formulas are invalid.
    if (formula.empty())
        return std::pair<bool, std::set<std::string> >(false, {});

    // Extract tokens from formula
    auto tokens = extract_tokens(formula);

    // Check the first token for syntax errors.
    auto firstToken = tokens.front();
    if (firstToken != "(" && !is_double(firstToken) && !is_valid_variable(firstToken)) {
        return std::pair<bool, std::set<std::string> >(false, {});
    }

    // Check the last token for syntax errors.
    auto lastToken = tokens.back();
    if (lastToken != ")" && !is_double(lastToken) && !is_valid_variable(lastToken)) {
        return std::pair<bool, std::set<std::string> >(false, {});
    }

    // The number of left and right parentheses encountered, respectively.
    int *parentheses_count = int[]{0, 0};

    // The dependents of this formula.
    std::set<std::string> dependents;

    // Iterate over each token
    std::string previous_token;
    for (const auto &token : tokens) {

        if (!previous_token.empty()) {
            //TODO: Check tokens which follow opening parentheses or operators.
            //TODO: Check tokens which follow closing parentheses, numbers, or variables.
        }

        // Count parentheses.
        if (token == "(")
            parentheses_count[0]++;
        else if (token == ")") {
            parentheses_count[1]++;
        } else if (is_valid_variable(token)) {
            dependents.insert(token);
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