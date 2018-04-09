#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H

#include <string>

/**
 * Determines if the given variable is formatted correctly.
 *
 * A valid variable is one which starts with a letter or underscore,
 * and is followed by zero or more letters, underscores, or numbers.
 * @param variable The variable to check.
 * @return True if the variable is valid, false otherwise.
 */
static bool is_valid_variable(const std::string &variable);

/**
 * Determines if the given string is a double.
 * @param str The string.
 * @return True if the string is a double, false otherwise.
 */
static bool is_double(const std::string &str);

#endif //PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H
