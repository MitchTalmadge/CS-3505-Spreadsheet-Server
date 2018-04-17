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
  // Before we write the new value in, save the old value to the undo stack for this spreadsheet.
  std::old_value = cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)];
  
  cell_value old_cell;
  old_cell.cell_name = cell_name;
  old_cell.value = old_value;

  // Push value onto the undo history.
  undo_history_.push(old_cell);

    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = contents;
}

void spreadsheet::undo() {
  // If history exists - history is maintained for current server session of this spreadsheet.
  if (!undo_history_.empty()) {
    cell_value undo = undo_history_.top();

    std::string cell_name = undo.cell_name;
    std::string value = undo.value;

    // Set value ourselves and DON'T place current value on undo stack - undos are destructive.
    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = value;

    // Remove element from undo stack.
    undo_history_.pop();
  }
}
