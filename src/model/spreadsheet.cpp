#include "spreadsheet.h"

spreadsheet::spreadsheet() = default;

spreadsheet::spreadsheet(const std::string &file_path) {

}

void spreadsheet::save_to_file(const std::string &file_path) {

}

std::string spreadsheet::get_cell_contents( const std::string &  cell_name) {
    return cell_contents_[cell_name];
}

void spreadsheet::set_cell_contents( const std::string &  cell_name, const std::string & contents) {
    cell_contents_[cell_name] = contents;
}
