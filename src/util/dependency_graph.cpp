#include "dependency_graph.h"

int spreadsheet::DependencyGraph::size() {
    return size;
}

std::set <std::string> spreadsheet::DependencyGraph::getDependees(std::string node) {
    return std::set<std::string>(dependees[node]);
}

std::set <std::string> spreadsheet::DependencyGraph::getDependents(std::string node) {
    return std::set<std::string>(dependents[node]);
}

void spreadsheet::DependencyGraph::addDependency(std::string node, std::string dependentNode) {
    std::set <std::string> nodeDependees = dependees[dependentNode];
    std::set <std::string> nodeDependents = dependents[node];

    // Attempt insertion of the node and its dependent.
    bool inserted = nodeDependees.insert(node).second;
    inserted &= nodeDependents.insert(dependentNode).second;

    // Increase number of pairs if insertion was successful.
    if (inserted)
        size++;
}

void spreadsheet::DependencyGraph::removeDependency(std::string node, std::string dependentNode) {
    std::set <std::string> nodeDependees = dependees[dependentNode];
    std::set <std::string> nodeDependents = dependents[node];

    // Remove node
    std::set<std::string>::iterator nodeToRemove = nodeDependees.find(node);
    if (nodeToRemove != nodeDependees.end()) {
        nodeDependees.erase(nodeToRemove);
    }

    // Remove node's dependent
    nodeToRemove = nodeDependents.find(dependentNode);
    if (nodeToRemove != nodeDependents.end()) {
        nodeDependees.erase(nodeToRemove);
    }
}