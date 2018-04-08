#include <gtest/gtest.h>
#include <model/dependency_graph.h>

/**
 * Tests that the dependency graph starts empty.
 */
TEST(DependencyGraphTest, GraphStartsEmpty) {
    dependency_graph graph;

    EXPECT_EQ(0, graph.size());
}