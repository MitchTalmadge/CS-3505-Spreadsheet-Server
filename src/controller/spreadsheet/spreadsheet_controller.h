#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_CONTROLLER_H

#include <string>
#include <vector>
#include <model/spreadsheet.h>
#include <controller/data_container.h>

/**
 * Controller for all the available and loaded spreadsheets.
 */
class spreadsheet_controller {

    /**
     * Contains all spreadsheets which are active, meaning they have been loaded by a client.
     * Maps spreadsheet file names to spreadsheet instances.
     */
    std::map<const std::string, spreadsheet *> active_spreadsheets_;

    /**
     * The data container that interfaces between the network controller and spreadsheet controller.
     */
    data_container &data_container_ = data_container::get_instance();

    /**
     * Private constructor for singleton pattern.
     */
    spreadsheet_controller();

    /**
     * Private destructor for singleton pattern.
     */
    ~spreadsheet_controller();

    /**
     * The work-loop for all active spreadsheets.
     */
    void work();

    /**
     * Parses an inbound message for a given spreadsheet.
     * @param message The message to parse.
     * @param spreadsheet_name The spreadsheet file name.
     * @param sheet The spreadsheet instance.
     */
    void parse_inbound_message(const std::string &message, const std::string &spreadsheet_name, spreadsheet &sheet);

public:

    /**
     * @return The singleton instance of this controller.
     */
    static spreadsheet_controller &get_instance();

    /**
     * @return Valid connect_accepted message for the current state of the server.
     */
    std::string get_spreadsheets();

    /**
     * Handle the opening of a new spreadsheet, adding to the list
     * of active models or creating a new spreadsheet entirely as necessary.
     *
     * @param spreadsheet Name of spreadsheet that client wants to load.
     * @return The full_state message for the given spreadsheet.
     */
    std::string get_spreadsheet(std::string spreadsheet);

    /**
     * Deleted copy constructor since this is a singleton.
     */
    spreadsheet_controller(spreadsheet_controller const &) = delete;

    /**
     * Deleted assignment operator since this is a singleton.
     */
    void operator=(spreadsheet_controller const &)  = delete;

    /**
     * Determines if the given cell name is formatted correctly.
     *
     * A valid cell name is one which starts with a letter or underscore,
     * and is followed by zero or more letters, underscores, or numbers.
     * @param cell_name The cell name to check.
     * @return True if the cell name is valid, false otherwise.
     */
    static bool is_valid_cell_name(const std::string &cell_name);

    /**
     * Normalizes the given cell name for consistency.
     *
     * All cell names will be converted to uppercase.
     *
     * @param cellName The name of the cell to normalize.
     * @return The normalized cell name.
     */
    static std::string normalize_cell_name(std::string cellName);

    /**
     * Determines if the given string is a double.
     * @param str The string.
     * @return True if the string is a double, false otherwise.
     */
    static bool is_double(const std::string &str);

};


#endif
