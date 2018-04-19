#include <gtest/gtest.h>
#include <model/spreadsheet.h>
#include <fstream>

TEST(spreadsheet, load_from_file) {
  // Load spreadsheet.
  spreadsheet sheet("test/resources/save_file_1.sprd");

  // Check contents.
  EXPECT_EQ("10", sheet.get_cell_contents("A1"));
  EXPECT_EQ("=A1+4", sheet.get_cell_contents("A25"));
  EXPECT_EQ("=A25-A1", sheet.get_cell_contents("C98"));
  EXPECT_EQ("hello world!", sheet.get_cell_contents("D5"));
  EXPECT_EQ("", sheet.get_cell_contents("A2")); // Random empty cell.
}

TEST(spreadsheet, save_to_file) {
  // Make a spreadsheet.
  spreadsheet sheet;

  sheet.set_cell_contents("A1", "10");
  sheet.set_cell_contents("A25", "=A1+4");
  sheet.set_cell_contents("C98", "=A25-A1");
  sheet.set_cell_contents("D5", "hello world!");

  // Save it.
  sheet.save_to_file("test/resources/save_file_1_out.sprd");

  // Read files.
  std::ifstream original("test/resources/save_file_1.sprd");
  std::ifstream saved("test/resources/save_file_1_out.sprd");

  // Compare contents.
  EXPECT_EQ(std::string(std::istreambuf_iterator<char>(original), std::istreambuf_iterator<char>()),
            std::string(std::istreambuf_iterator<char>(saved), std::istreambuf_iterator<char>()));
}