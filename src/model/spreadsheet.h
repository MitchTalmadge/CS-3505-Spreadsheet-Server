#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_H

#include <string>
#include <map>
#include <model/dependency_graph.h>

/**
 * Represents one collaborative Spreadsheet and all its contents.
 */
class spreadsheet {

    /**
     * Determines if the spreadsheet has been changed at all since last opened/saved.
     */
    bool changed_ = false;

    /**
     * Contains relations between cells in the spreadsheet.
     * Each node in the graph is the name of a cell.
     */
    dependency_graph dependency_graph_;

    /**
     * Contains the contents of each cell in the spreadsheet.
     * Maps cell names to cell contents.
     */
    std::map<std::string, std::string> cell_contents_;

public:

    /**
     * Creates a new, empty spreadsheet.
     */
    spreadsheet();

    /**
     * Loads a spreadsheet from a file.
     * @param file_path The path to the JSON spreadsheet file.
     */
    spreadsheet(std::string &file_path);

    /**
     * Saves this spreadsheet to a JSON file.
     * After a successful save, the spreadsheet is no longer considered "changed."
     * @param file_path Where to save the spreadsheet, including the file's name and extension.
     */
    void save_to_file(std::string &file_path);

    /**
     * Retrieves the contents of a specific cell.
     * @param cell_name The name of the cell.
     * @return The contents of the cell.
     */
    std::string get_cell_contents(std::string cell_name);

    /**
     * Sets the contents of a specific cell.
     * @param cell_name The name of the cell.
     * @param contents The contents of the cell.
     */
    void set_cell_contents(std::string cell_name, std::string contents);

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_SPREADSHEET_H
