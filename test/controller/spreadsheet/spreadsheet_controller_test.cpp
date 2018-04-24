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
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("a1"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("Z1"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("a1"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("A1"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("D54"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("B10"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("B25"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("C99"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("C98"));
    EXPECT_TRUE(spreadsheet_controller::is_valid_cell_name("z2"));
}

/**
 * Tests that invalid cell names are marked as invalid.
 */
TEST(spreadsheet_controller, is_valid_cell_name__Invalid_Names) {
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("a"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("A"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("D"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("Z"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("Z0"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("A01"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("A09"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("A100"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("B950"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("_10"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("D___9"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("aB_9_d"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("MITCH"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("MAAAAAAAAaaaHHHK_1337"));
    EXPECT_FALSE(spreadsheet_controller::is_valid_cell_name("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"));
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