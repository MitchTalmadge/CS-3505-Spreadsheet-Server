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
bool is_valid_variable(const std::string &variable);

/**
 * Determines if the given string is a double.
 * @param str The string.
 * @return True if the string is a double, false otherwise.
 */
bool is_double(const std::string &str);

/**
 * Normalizes the given cell name for consistency.
 *
 * All cell names will be converted to uppercase.
 *
 * @param cellName The name of the cell to normalize.
 * @return The normalized cell name.
 */
std::string normalize_cell_name(std::string cellName);

#endif //PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H
