#include <gtest/gtest.h>
#include <model/dependency_graph.h>

TEST(DependencyGraphTest, GraphStartsEmpty) {
    dependency_graph graph;

    EXPECT_EQ(0, graph.size());
}