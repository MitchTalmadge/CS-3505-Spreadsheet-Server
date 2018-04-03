#include "dependency_graph.h"

int spreadsheet::DependencyGraph::size() {
    //TODO
    return 0;
}

std::vector<std::string> spreadsheet::DependencyGraph::getDependees(std::string node) {
    //TODO
    return dependees.at(node);
}

std::vector<std::string> spreadsheet::DependencyGraph::getDependees(std::string node) {
    //TODO
    return dependents.at(node);
}

void spreadsheet::DependencyGraph::addDependency(std::string node, std::string dependentNode) {
    //TODO
}

void spreadsheet::DependencyGraph::removeDependency(std::string node, std::string dependentNode) {
    //TODO
}