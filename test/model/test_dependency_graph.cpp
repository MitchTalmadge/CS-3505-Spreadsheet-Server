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

    EXPECT_EQ(1, graph.size());

    auto dependent_node_dependees = graph.get_dependees("A2");
    EXPECT_EQ(1, dependent_node_dependees.size());
    EXPECT_TRUE(dependent_node_dependees.find("A1") != dependent_node_dependees.end());

    auto node_dependents = graph.get_dependents("A1");
    EXPECT_EQ(1, node_dependents.size());
    EXPECT_TRUE(node_dependents.find("A2") != node_dependents.end());
}