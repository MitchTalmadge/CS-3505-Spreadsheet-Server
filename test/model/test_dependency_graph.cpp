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

    auto a2_dependees = graph.get_dependees("A2");
    EXPECT_EQ(1, a2_dependees.size());
    EXPECT_TRUE(a2_dependees.find("A1") != a2_dependees.end());

    auto a1_dependents = graph.get_dependents("A1");
    EXPECT_EQ(1, a1_dependents.size());
    EXPECT_TRUE(a1_dependents.find("A2") != a1_dependents.end());
}

/**
 * Tests that multiple dependencies can be added and retrieved from the graph.
 */
TEST(DependencyGraphTest, AddMultipleDependencies) {
    dependency_graph graph;

    graph.add_dependency("A1", "A2");
    graph.add_dependency("A1", "A3");
    graph.add_dependency("A1", "A4");
    graph.add_dependency("A2", "A3");

    EXPECT_EQ(4, graph.size());

    auto a2_dependees = graph.get_dependees("A2");
    EXPECT_EQ(1, a2_dependees.size());
    EXPECT_TRUE(a2_dependees.find("A1") != a2_dependees.end());

    auto a3_dependees = graph.get_dependees("A3");
    EXPECT_EQ(2, a3_dependees.size());
    EXPECT_TRUE(a3_dependees.find("A1") != a3_dependees.end());
    EXPECT_TRUE(a3_dependees.find("A2") != a3_dependees.end());

    auto a1_dependents = graph.get_dependents("A1");
    EXPECT_EQ(3, a1_dependents.size());
    EXPECT_TRUE(a1_dependents.find("A2") != a1_dependents.end());
    EXPECT_TRUE(a1_dependents.find("A3") != a1_dependents.end());
    EXPECT_TRUE(a1_dependents.find("A4") != a1_dependents.end());

    auto a2_dependents = graph.get_dependents("A2");
    EXPECT_EQ(1, a2_dependents.size());
    EXPECT_TRUE(a2_dependents.find("A3") != a2_dependents.end());
}

/**
 * Tests that multiple dependencies can be added and retrieved from the graph.
 */
TEST(DependencyGraphTest, AddExistingDependencies) {
    dependency_graph graph;

    // Initial
    graph.add_dependency("A1", "A2");
    graph.add_dependency("A1", "A3");

    // Repeat
    graph.add_dependency("A1", "A2");
    graph.add_dependency("A1", "A3");

    // Only 2 should have been added
    EXPECT_EQ(2, graph.size());

    auto a2_dependees = graph.get_dependees("A2");
    EXPECT_EQ(1, a2_dependees.size());
    EXPECT_TRUE(a2_dependees.find("A1") != a2_dependees.end());

    auto a3_dependees = graph.get_dependees("A3");
    EXPECT_EQ(1, a3_dependees.size());
    EXPECT_TRUE(a3_dependees.find("A1") != a3_dependees.end());

    auto a1_dependents = graph.get_dependents("A1");
    EXPECT_EQ(2, a1_dependents.size());
    EXPECT_TRUE(a1_dependents.find("A2") != a1_dependents.end());
    EXPECT_TRUE(a1_dependents.find("A3") != a1_dependents.end());
}

/**
 * Tests that a dependency can be removed after being added.
 */
TEST(DependencyGraphTest, RemoveOneDependency) {
    dependency_graph graph;

    graph.add_dependency("A1", "A2");

    graph.remove_dependency("A1", "A2");

    EXPECT_EQ(0, graph.size());

    auto a2_dependees = graph.get_dependees("A2");
    EXPECT_EQ(0, a2_dependees.size());
    EXPECT_TRUE(a2_dependees.find("A1") == a2_dependees.end());

    auto a1_dependents = graph.get_dependents("A1");
    EXPECT_EQ(0, a1_dependents.size());
    EXPECT_TRUE(a1_dependents.find("A2") == a1_dependents.end());
}