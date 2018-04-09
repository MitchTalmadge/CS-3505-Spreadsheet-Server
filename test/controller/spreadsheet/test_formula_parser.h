#ifndef PIGRAMMERS_SPREADSHEET_SERVER_TEST_FORMULA_PARSER_H
#define PIGRAMMERS_SPREADSHEET_SERVER_TEST_FORMULA_PARSER_H

#include <string>
#include <set>

/**
 * Utility testing method to check if two sets are exactly equal in size and contents.
 *
 * @param set The set to check against.
 * @param correctSet The set with the correct items and size.
 */
void compare_set(const std::set<std::string> &set, const std::set<std::string> &correctSet);

#endif //PIGRAMMERS_SPREADSHEET_SERVER_TEST_FORMULA_PARSER_H
