#include <model/spreadsheet.h>
#include <gtest/gtest.h>

/**
 * Tests the ability to set and retrieve the contents of a single cell.
 */
TEST(SpreadsheetTest, SetOneCell) {
    spreadsheet sheet;

    sheet.set_cell_contents("A1", "test");
    EXPECT_EQ("test", sheet.get_cell_contents("A1"));
}

/**
 * Tests that cell names are treated the same regardless of case.
 */
TEST(SpreadsheetTest, CaseInsensitivity) {
    spreadsheet sheet;

    sheet.set_cell_contents("A1", "test");
    EXPECT_EQ("test", sheet.get_cell_contents("A1"));
    EXPECT_EQ("test", sheet.get_cell_contents("a1"));
}

/**
 * Tests the ability to update the contents of an existing cell.
 */
TEST(SpreadsheetTest, UpdateExistingCell) {
    spreadsheet sheet;

    sheet.set_cell_contents("A1", "1");
    sheet.set_cell_contents("A1", "2");

    EXPECT_EQ("2", sheet.get_cell_contents("A1"));
}

/**
 * Tests the ability to retrieve a cell that has not been explicitly set.
 * The contents should be empty.
 */
TEST(SpreadsheetTest, GetEmptyCell) {
    spreadsheet sheet;

    EXPECT_EQ("", sheet.get_cell_contents("B10"));
}

/**
 * Tests the ability to make a cell empty after it has had contents written to it.
 */
TEST(SpreadsheetTest, MakeCellEmpty) {
    spreadsheet sheet;

    sheet.set_cell_contents("A1", "cat");
    sheet.set_cell_contents("A1", "");

    EXPECT_EQ("", sheet.get_cell_contents("A1"));
}

