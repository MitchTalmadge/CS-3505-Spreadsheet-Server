#include "spreadsheet.h"
#include <controller/spreadsheet/spreadsheet_controller.h>
#include <algorithm>

spreadsheet::spreadsheet() = default;

spreadsheet::spreadsheet(const std::string &file_path) {

}

void spreadsheet::save_to_file(const std::string &file_path) {

}

std::string spreadsheet::get_cell_contents(const std::string &cell_name) {
    return cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)];
}

void spreadsheet::set_cell_contents(const std::string &cell_name, const std::string &contents) {
    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = contents;
}

void spreadsheet::focus_cell(int socket_id, const std::string &cell_name) {
    focused_cells_[socket_id] = cell_name;
}

void spreadsheet::unfocus_cell(int socket_id) {
    focused_cells_.erase(socket_id);
}
