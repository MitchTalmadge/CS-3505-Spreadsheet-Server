#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H

#include <string>

/**
 * Determines if the given cell name is formatted correctly.
 *
 * A valid cell name is one which starts with a letter or underscore,
 * and is followed by zero or more letters, underscores, or numbers.
 * @param cell_name The cell name to check.
 * @return True if the cell name is valid, false otherwise.
 */
bool is_valid_cell_name(const std::string &cell_name);

/**
 * Normalizes the given cell name for consistency.
 *
 * All cell names will be converted to uppercase.
 *
 * @param cellName The name of the cell to normalize.
 * @return The normalized cell name.
 */
std::string normalize_cell_name(std::string cellName);

/**
 * Determines if the given string is a double.
 * @param str The string.
 * @return True if the string is a double, false otherwise.
 */
bool is_double(const std::string &str);


#endif //PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_UTILS_H
