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
  std::string old_value = get_cell_contents(cell_name);

  cell_value old_cell;
  old_cell.cell_name = cell_name;
  old_cell.value = old_value;
  old_cell.is_revert = false;

  // Push value onto the undo history.
  undo_history_.push(old_cell);

  // Push old value onto this cell's revert.
  revert_history_[cell_name].push(old_value);

    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = contents;
}

void spreadsheet::focus_cell(int socket_id, const std::string &cell_name) {
    focused_cells_[socket_id] = cell_name;
}

void spreadsheet::unfocus_cell(int socket_id) {
    focused_cells_.erase(socket_id);
}

void spreadsheet::undo() {
  // If history exists - history is maintained for current server session of this spreadsheet.
  if (!undo_history_.empty()) {
    cell_value undo = undo_history_.top();

    bool is_revert = undo.is_revert;
    std::string cell_name = undo.cell_name;
    std::string value = undo.value;

    // Set value ourselves and DON'T place current value on undo stack - undos are destructive.
    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = value;

    // If the undo is not undoing a revert, we need to pop from the corresponding revert stack
    // to maintain consistency.
    if (!is_revert) {
      revert_history_[cell_name].pop();
    }

    // Remove element from undo stack.
    undo_history_.pop();
  }
}

void spreadsheet::revert(const std::string &cell_name) {
  // Check if a revert history exists for this cell.
  if (!revert_history_[cell_name].empty()) {
    // Get the value we should revert to.
    std::string revert_value = revert_history_[cell_name].top();

    // Get the current value to save to the UNDO stack.
    std::string current = get_cell_contents(cell_name);

    cell_value current_value;
    current_value.cell_name = cell_name;
    current_value.value = current;
    current_value.is_revert = true;

    // Set new value.
    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = revert_value;

    // Push the current value to undo stack.
    undo_history_.push(current_value);
  }
}