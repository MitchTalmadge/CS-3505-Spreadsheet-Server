#include "dependency_graph.h"

int dependency_graph::size() const {
    return size_;
}

std::set <std::string> dependency_graph::get_dependees(const std::string& node) {
    return std::set<std::string>(dependees_[node]);
}

std::set <std::string> dependency_graph::get_dependents(const std::string& node) {
    return std::set<std::string>(dependents_[node]);
}

void dependency_graph::add_dependency(const std::string& node, const std::string& dependent_node) {
    std::set <std::string> dependent_node_dependees = dependees_[dependent_node];
    std::set <std::string> node_dependents = dependents_[node];

    // Attempt insertion of the node and its dependent.
    bool inserted = dependent_node_dependees.insert(node).second;
    inserted &= node_dependents.insert(dependent_node).second;

    // Increase number of pairs if insertion was successful.
    if (inserted)
        size_++;
}

void dependency_graph::remove_dependency(const std::string& node, const std::string& dependent_node) {
    std::set <std::string> node_dependees = dependees_[dependent_node];
    std::set <std::string> node_dependents = dependents_[node];

	auto removed = false;

    // Remove node
    auto nodeToRemove = node_dependees.find(node);
    if (nodeToRemove != node_dependees.end()) {
        node_dependees.erase(nodeToRemove);
        removed = true;
    }

    // Remove node's dependent
    nodeToRemove = node_dependents.find(dependent_node);
    if (nodeToRemove != node_dependents.end()) {
        node_dependees.erase(nodeToRemove);
        removed = true;
    }

    // Decrease number of pairs if removal was successful.
    if(removed) {
        size_--;
    }
}