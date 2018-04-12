#include <controller/spreadsheet/spreadsheet_controller.h>
#include <gtest/gtest.h>

/**
 * Tests that cell names are properly normalized.
 */
TEST(spreadsheet_controller, normalize_cell_name__Comprehensive) {
    EXPECT_EQ("A1", spreadsheet_controller::normalize_cell_name("A1"));
    EXPECT_EQ("A1", spreadsheet_controller::normalize_cell_name("a1"));
    EXPECT_EQ("A10", spreadsheet_controller::normalize_cell_name("A10"));
    EXPECT_EQ("A10", spreadsheet_controller::normalize_cell_name("a10"));
    EXPECT_EQ("MITCH", spreadsheet_controller::normalize_cell_name("mitch"));
    EXPECT_EQ("JIAHU___I", spreadsheet_controller::normalize_cell_name("jIAhU___i"));
}

/**
 * Tests that valid cell names are marked as valid.
 */
TEST(spreadsheet_controller, is_valid_cell_name__Valid_Names) {
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("a"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("Z"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("a1"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("A1"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("D54"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("B10"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("_10"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("D___9"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("aB_9_d"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("MITCH"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("MAAAAAAAAaaaHHHK_1337"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"));
}

/**
 * Tests that invalid cell names are marked as invalid.
 */
TEST(spreadsheet_controller, is_valid_cell_name__Invalid_Names) {
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("2a"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("a10!"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("a-5"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("-10"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("i :) want :) to :) die"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("10_5"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890$"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("^ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("^ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890$"));
}