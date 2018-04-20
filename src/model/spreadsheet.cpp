#include "spreadsheet.h"
#include <controller/spreadsheet/spreadsheet_controller.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

spreadsheet::spreadsheet() {
  loaded_ = true;
}

spreadsheet::spreadsheet(const std::string &file_path) {
  // Read in file as string
  auto abs_file_path = boost::filesystem::absolute(boost::filesystem::path(file_path)).string();
  std::cout << "Loading spreadsheet file: " << abs_file_path << std::endl;
  std::ifstream input_stream(abs_file_path);

  if (!input_stream.is_open()) {
    loaded_ = false;
    return;
  }

  auto file_contents = std::string(std::istreambuf_iterator<char>(input_stream), std::istreambuf_iterator<char>());

  // Check for header.
  if (!boost::starts_with(file_contents, "spreadsheet|")) {
    loaded_ = false;
    return;
  }

  // Extract cells.
  std::vector<std::string> split;
  auto file_contents_minus_header = file_contents.substr(12);
  boost::split(split, file_contents_minus_header, boost::is_any_of(":"));

  // Make sure that an even number of items were retrieved, otherwise one cell does not have a matching contents.
  if (split.size() % 2 != 0) {
    loaded_ = false;
    return;
  }

  // Iterate over each split item and map cells.
  for (auto item = split.begin(); item != split.end(); ++item) {
    auto &key = *item;
    auto &value = *++item;
    cell_contents_[key] = value;
  }

  loaded_ = true;
}

void spreadsheet::save_to_file(const std::string &file_path) {
  // Extract the filename and directories.
  unsigned int path_split_index = file_path.find_last_of('/');

  std::string file_name_part;
  std::string directory_part;

  if (path_split_index == std::string::npos) {
    file_name_part = file_path;
  } else {
    file_name_part = file_path.substr(path_split_index);
    directory_part = file_path.substr(0, path_split_index);
  }

  // Create the directories as needed.
  if (!directory_part.empty()) {
    boost::filesystem::create_directories(directory_part);
  }

  // Create file contents.
  std::string contents = "spreadsheet|";

  for (auto &&item : cell_contents_) {
    // Skip empty cells
    if (item.second.empty())
      continue;

    contents += item.first + ":" + item.second + ":";
  }

  if (boost::ends_with(contents, ":"))
    contents.erase(contents.size() - 1, 1);

  // Write contents to file.
  auto abs_file_path = boost::filesystem::absolute(boost::filesystem::path(file_path)).string();
  std::cout << "Saving spreadsheet file: " << abs_file_path << std::endl;
  std::ofstream output_stream(abs_file_path);
  output_stream << contents;
  output_stream.close();
}

std::string spreadsheet::get_cell_contents(const std::string &cell_name) const {
  auto iter = cell_contents_.find(cell_name);
  if (iter != cell_contents_.end())
    return iter->second;
  return "";
}

void spreadsheet::set_cell_contents(const std::string &cell_name, const std::string &contents) {
  // Before we write the new contents in, save the old contents to the undo stack for this spreadsheet.
  std::string old_history = get_cell_contents(cell_name);

  cell_history old_cell;
  old_cell.cell_name = cell_name;
  old_cell.contents = old_history;
  old_cell.is_revert = false;

  // Push contents onto the undo history.
  undo_history_.push(old_cell);

  // Push old contents onto this cell's revert.
  revert_history_[cell_name].push(old_history);

  cell_contents_[cell_name] = contents;
}

std::map<std::string, std::string> spreadsheet::get_non_empty_cells() const {
  std::map<std::string, std::string> map;

  for (auto &&item : cell_contents_) {
    if (item.second.empty())
      continue;

    map[item.first] = item.second;
  }

  return map;
}

void spreadsheet::focus_cell(int socket_id, const std::string &cell_name) {
  focused_cells_[socket_id] = cell_name;
}

void spreadsheet::unfocus_cell(int socket_id) {
  focused_cells_.erase(socket_id);
}

boost::optional<std::pair<std::string, std::string> > spreadsheet::undo() {
  // If history exists - history is maintained for current server session of this spreadsheet.
  if (!undo_history_.empty()) {
    cell_history undo = undo_history_.top();

    // Get current value of cell to put back in the revert stack if necessary.
    std::string current_contents = cell_contents_[spreadsheet_controller::normalize_cell_name(undo.cell_name)];

    // Set contents ourselves and DON'T place current contents on undo stack - undos are destructive.
    cell_contents_[spreadsheet_controller::normalize_cell_name(undo.cell_name)] = undo.contents;

    // If the undo is not undoing a revert, we need to pop from the corresponding revert stack
    // to maintain consistency.
    if (!undo.is_revert) {
      revert_history_[undo.cell_name].pop();
    } else {
      // If the undo is a revert, push the current value onto the revert stack for that cell, thus fully
      // undoing the revert.
      revert_history_[undo.cell_name].push(current_contents);
    }

    // Remove element from undo stack.
    undo_history_.pop();

    return std::make_pair(undo.cell_name, undo.contents);
  }

  return boost::none;
}

boost::optional<std::pair<std::string, std::string> > spreadsheet::revert(const std::string &cell_name) {
  // Check if a revert history exists for this cell.
  if (!revert_history_[cell_name].empty()) {
    // Get the contents we should revert to.
    std::string revert_contents = revert_history_[cell_name].top();

    // Get the current contents to save to the UNDO stack.
    std::string current_contents = get_cell_contents(cell_name);

    cell_history current_history;
    current_history.cell_name = cell_name;
    current_history.contents = current_contents;
    current_history.is_revert = true;

    // Set new contents.
    cell_contents_[spreadsheet_controller::normalize_cell_name(cell_name)] = revert_contents;

    // Push the current contents to undo stack.
    undo_history_.push(current_history);

    return std::make_pair(cell_name, revert_contents);
  }

  return boost::none;
}
bool spreadsheet::is_loaded() const {
  return loaded_;
}
