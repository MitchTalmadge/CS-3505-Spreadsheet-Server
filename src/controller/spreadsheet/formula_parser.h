#ifndef PIGRAMMERS_SPREADSHEET_SERVER_FORMULA_PARSER_H
#define PIGRAMMERS_SPREADSHEET_SERVER_FORMULA_PARSER_H

#include <string>
#include <set>
#include <vector>

class formula_parser {

    /**
     * Extracts all tokens from the given formula.
     * @param formula The formula.
     * @return The tokens extracted in the order they were encountered.
     */
    static std::vector<std::string> extract_tokens(const std::string &formula);

public:

    /**
     * Determines if the given formula is formatted correctly.
     * @param formula The formula to check, such as "=A1 + 10"
     * @return True if the formula is formatted correctly, false otherwise.
     */
    static bool is_valid(std::string formula);

    /**
     * Finds the names of all cells that depend on this formula.
     * @param formula The formula to search.
     * @return A set containing the names of the cells which depend on the formula.
     */
    static std::set<std::string> find_dependents(const std::string &formula);

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_FORMULA_PARSER_H
