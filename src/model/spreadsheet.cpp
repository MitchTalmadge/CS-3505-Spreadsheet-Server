#include "spreadsheet.h"
#include <algorithm>

spreadsheet::spreadsheet() = default;

spreadsheet::spreadsheet(const std::string &file_path) {

}

void spreadsheet::save_to_file(const std::string &file_path) {

}

std::string spreadsheet::get_cell_contents(const std::string &cell_name) {
    return cell_contents_[normalize_cell_name(cell_name)];
}

void spreadsheet::set_cell_contents(const std::string &cell_name, const std::string &contents) {
    cell_contents_[normalize_cell_name(cell_name)] = contents;
}

std::string spreadsheet::normalize_cell_name(std::string cellName) {
    std::transform(cellName.begin(), cellName.end(), cellName.begin(), ::toupper);
    return cellName;
}
