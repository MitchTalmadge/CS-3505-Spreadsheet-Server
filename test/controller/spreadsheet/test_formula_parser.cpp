#include <controller/spreadsheet/formula_parser.h>
#include <gtest/gtest.h>

/**
 * Tests the rule that formulas must contain at least one token.
 */
TEST(FormulaParserTest, EmptyFormula) {
    EXPECT_FALSE(formula_parser::is_valid(""));
    EXPECT_FALSE(formula_parser::is_valid("    "));
    EXPECT_FALSE(formula_parser::is_valid("              "));
    EXPECT_FALSE(formula_parser::is_valid("()"));
    EXPECT_FALSE(formula_parser::is_valid("(())"));
}

/**
 * Tests the rule that formulas may allow floating point numbers.
 */
TEST(FormulaParserTest, FloatingPoint) {
    EXPECT_TRUE(formula_parser::is_valid("10.5 + 5"));
    EXPECT_TRUE(formula_parser::is_valid("11.123456 - 1.23445 * (150.3984 / 1082.43434)"));
}

/**
 * Tests the rule that formulas may allow scientifically formatted numbers.
 */
TEST(FormulaParserTest, ScientificNotation) {
    EXPECT_TRUE(formula_parser::is_valid("1.05E1 + 5"));
    EXPECT_TRUE(formula_parser::is_valid("1.24E20 - 1.5E9 * (1.09E6 / 0.000924E4)"));
}

/**
 * Tests the rule that when reading tokens from left to right, at no point should the number of
 * closing parentheses seen so far be greater than the number of opening parentheses seen so far.
 */
TEST(FormulaParserTest, TooManyRightParentheses) {
    EXPECT_FALSE(formula_parser::is_valid("(10 + 5))"));
    EXPECT_FALSE(formula_parser::is_valid("((10 + 5)))"));
    EXPECT_FALSE(formula_parser::is_valid("((10 + 5) + (5 + (5 * 10)) 10))"));
}

/**
 * Tests the rule that the total number of opening parentheses must equal the total number of closing parentheses.
 */
TEST(FormulaParserTest, BalancedParentheses) {
    EXPECT_TRUE(formula_parser::is_valid("(10 + 5)"));
    EXPECT_TRUE(formula_parser::is_valid("((10 - 5))"));

    EXPECT_FALSE(formula_parser::is_valid("(10 + 5"));
    EXPECT_FALSE(formula_parser::is_valid("((10 / 5)"));
    EXPECT_FALSE(formula_parser::is_valid("(((10 - 5"));
    EXPECT_FALSE(formula_parser::is_valid("(((10 + 5) * (5 + (5 * 10)) 10)"));
}

/**
 * Tests the rule that the first token of a formula must be a number, a variable, or an opening parenthesis.
 */
TEST(FormulaParserTest, StartingToken) {
    EXPECT_TRUE(formula_parser::is_valid("(10 + 5)")); // Start with opening parenthesis
    EXPECT_TRUE(formula_parser::is_valid("10 - 5")); // Start with number
    EXPECT_TRUE(formula_parser::is_valid("A5 * 5")); // Start with variable

    EXPECT_FALSE(formula_parser::is_valid(")10 + 5("));
    EXPECT_FALSE(formula_parser::is_valid("/ 10 / 5"));
    EXPECT_FALSE(formula_parser::is_valid("+ 5 * 10"));
    EXPECT_FALSE(formula_parser::is_valid("$10 - 5"));
    EXPECT_FALSE(formula_parser::is_valid("+(10 + 5) * 5"));
}

/**
 * Tests the rule that the last token of an expression must be a number, a variable, or a closing parenthesis.
 */
TEST(FormulaParserTest, EndingToken) {
    EXPECT_TRUE(formula_parser::is_valid("(10 + 5)")); // End with closing parenthesis.
    EXPECT_TRUE(formula_parser::is_valid("10 - 5")); // End with number.
    EXPECT_TRUE(formula_parser::is_valid("5 * A5")); // End with variable.

    EXPECT_FALSE(formula_parser::is_valid("(10 - 5("));
    EXPECT_FALSE(formula_parser::is_valid("10 + 5 %"));
    EXPECT_FALSE(formula_parser::is_valid("5 / 10 + "));
    EXPECT_FALSE(formula_parser::is_valid("10 - 5$"));
    EXPECT_FALSE(formula_parser::is_valid("5 * (10 + 5)+"));
}

/**
 * Tests the rule that any token that immediately follows an opening parenthesis
 * or an operator must be either a number, a variable, or an opening parenthesis.
 */
TEST(FormulaParserTest, TokenFollowingParenthesis) {
    // Parenthesis
    EXPECT_TRUE(formula_parser::is_valid("(10 + 5)")); // Number.
    EXPECT_TRUE(formula_parser::is_valid("(A5 / 5)")); // Variable.
    EXPECT_TRUE(formula_parser::is_valid("((10 * 5))")); // Opening Parenthesis.
    EXPECT_TRUE(formula_parser::is_valid("((((((((((10 - 5))))))))))")); // Opening Parenthesis.

    // Operator
    EXPECT_TRUE(formula_parser::is_valid("10 + 5")); // Number.
    EXPECT_TRUE(formula_parser::is_valid("10 * A5")); // Variable.
    EXPECT_TRUE(formula_parser::is_valid("10 - (10 + 5)")); // Opening Parenthesis.

    // Parenthesis
    EXPECT_FALSE(formula_parser::is_valid("()"));
    EXPECT_FALSE(formula_parser::is_valid("(; + 5)"));
    EXPECT_FALSE(formula_parser::is_valid("(+ 5)"));

    // Operator
    EXPECT_FALSE(formula_parser::is_valid("10 + /"));
    EXPECT_FALSE(formula_parser::is_valid("10 - *5"));
    EXPECT_FALSE(formula_parser::is_valid("(10 * )"));
    EXPECT_FALSE(formula_parser::is_valid("10 / )"));
}

/**
 * Tests the rule that any token that immediately follows a number, a variable,
 * or a closing parenthesis must be either an operator or a closing parenthesis.
 */
TEST(FormulaParserTest, TokenFollowingNumber) {
    // Number
    EXPECT_TRUE(formula_parser::is_valid("(10 + 5)")); // Closing Parenthesis.
    EXPECT_TRUE(formula_parser::is_valid("10 + 5")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("10 - 5")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("10 * 5")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("10 / 5")); // Operator.
    // Variable
    EXPECT_TRUE(formula_parser::is_valid("(10 + A5)")); // Closing Parenthesis.
    EXPECT_TRUE(formula_parser::is_valid("A5 + 10")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("A5 - 10")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("A5 * 10")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("A5 / 10")); // Operator.
    // Closing Parenthesis
    EXPECT_TRUE(formula_parser::is_valid("((10 + 5))")); // Closing Parenthesis.
    EXPECT_TRUE(formula_parser::is_valid("(5 + 5) + 10")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("(5 / 5) - 10")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("(5 / 5) * 10")); // Operator.
    EXPECT_TRUE(formula_parser::is_valid("(5 / 5) / 10")); // Operator.

    // Number
    EXPECT_FALSE(formula_parser::is_valid("10 AB 5"));
    EXPECT_FALSE(formula_parser::is_valid("10 = 5"));
    EXPECT_FALSE(formula_parser::is_valid("10 ... 5"));
    EXPECT_FALSE(formula_parser::is_valid("10 5"));
    // Variable
    EXPECT_FALSE(formula_parser::is_valid("A5 AB 5"));
    EXPECT_FALSE(formula_parser::is_valid("A5 = 5"));
    EXPECT_FALSE(formula_parser::is_valid("A5 ... 5"));
    EXPECT_FALSE(formula_parser::is_valid("A5 5"));
    // Closing Parenthesis
    EXPECT_FALSE(formula_parser::is_valid("(5 + 5) ^ 5"));
    EXPECT_FALSE(formula_parser::is_valid("(5 + 5) = 5"));
    EXPECT_FALSE(formula_parser::is_valid("(5 + 5) ... 5"));
    EXPECT_FALSE(formula_parser::is_valid("(5 + 5) (5 + 5)"));
    EXPECT_FALSE(formula_parser::is_valid("(5 + 5) 5"));
}

/**
 * Tests the validity of variables within formulas.
 */
TEST(FormulaParserTest, VariableValidity) {
    EXPECT_TRUE(formula_parser::is_valid("A + 1")); // One letter
    EXPECT_TRUE(formula_parser::is_valid("a + 1")); // One letter, lowercase
    EXPECT_TRUE(formula_parser::is_valid("A1 + 1")); // One letter and one number
    EXPECT_TRUE(formula_parser::is_valid("a1 + 1")); // One letter and one number, lowercase
    EXPECT_TRUE(formula_parser::is_valid("A1B + 1")); // Letter followed by number and letter.
    EXPECT_TRUE(formula_parser::is_valid("A1_ + 1")); // Letter followed by number and underscore.
    EXPECT_TRUE(formula_parser::is_valid("A_ + 1")); // Letter followed by underscore.
    EXPECT_TRUE(formula_parser::is_valid("_ + 1")); // Underscore only.
    EXPECT_TRUE(formula_parser::is_valid("____ + 1")); // Multiple Underscores.
    EXPECT_TRUE(formula_parser::is_valid("_B_D10_A123_ + 1")); // Mixed.
    EXPECT_TRUE(formula_parser::is_valid("A1234_5678B + 1")); // Mixed.
    EXPECT_TRUE(formula_parser::is_valid("A_1_B______C2 + 1")); // Mixed.

    EXPECT_FALSE(formula_parser::is_valid("1A + 5")); // Starts with number
    EXPECT_FALSE(formula_parser::is_valid("AB% + 5")); // Includes invalid characters
    EXPECT_FALSE(formula_parser::is_valid("AB$ + 5")); // Includes invalid characters
    EXPECT_FALSE(formula_parser::is_valid("A$B + 5")); // Includes invalid characters
    EXPECT_FALSE(formula_parser::is_valid("A_B&2 + 5")); // Includes invalid characters
}