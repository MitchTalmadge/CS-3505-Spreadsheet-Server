#include <gtest/gtest.h>
#include <model/dependency_graph.h>

/**
 * Tests that the dependency graph starts empty.
 */
TEST(DependencyGraphTest, GraphStartsEmpty) {
    dependency_graph graph;

    EXPECT_EQ(0, graph.size());
    EXPECT_EQ(0, graph.get_dependees("A1").size());
    EXPECT_EQ(0, graph.get_dependents("A1").size());
}

/**
 * Tests that a single dependency can be added and retrieved from the graph.
 */
TEST(DependencyGraphTest, AddOneDependency) {
    dependency_graph graph;

    graph.add_dependency("A1", "A2");

    EXPECT_EQ(1, graph.get_dependees("A2").size());
    EXPECT_EQ("A1", graph.get_dependees("A2").begin());

    EXPECT_EQ(1, graph.get_dependents("A1").size());
    EXPECT_EQ("A2", graph.get_dependents("A1").begin());
}