#ifndef PIGRAMMERS_SPREADSHEET_SERVER_SERVER_DEPENDENCY_GRAPH_H
#define PIGRAMMERS_SPREADSHEET_SERVER_SERVER_DEPENDENCY_GRAPH_H

#include <string>
#include <map>
#include <set>

/**
 * A Dependency Graph stores relational mappings between objects with the assumption that
 * one object depends in some way on another.
 *
 * This particular dependency graph stores strings.
 *
 * For example, in the case of a spreadsheet, one cell may depend on the value of another cell,
 * in which case the latter cell must be evaluated before the former.
 */
class dependency_graph {

    /**
     * Maps a node to a set of nodes which depend upon it.
     */
    std::map<std::string, std::set<std::string>> dependents_;

    /**
     * Maps a node to a set of nodes which it depends upon.
     */
    std::map<std::string, std::set<std::string>> dependees_;

    /**
     * The number of pairs in the graph.
     */
    int size_ = 0;

public:+

    /**
     * @return The number of ordered pairs in the graph.
     */
    int size() const;

    /**
     * Finds all dependents of the given node.
     * @param node The node.
     * @return Dependents of the node (Those nodes which depend on the given node).
     */
    std::set<std::string> get_dependents(const std::string &node);

    /**
     * Finds all dependees of the given node.
     * @param node The node.
     * @return Dependees of the node (Those nodes which the given node depends on).
     */
    std::set<std::string> get_dependees(const std::string &node);

    /**
     * Adds a dependency based on the given nodes.
     * @param node The reference node.
     * @param dependent_node The node that depends upon the reference node.
     */
    void add_dependency(const std::string &node, const std::string &dependent_node);

    /**
     * Removes a dependency based on the given nodes.
     * @param node The reference node.
     * @param dependent_node The node that depends on the reference node.
     */
    void remove_dependency(const std::string &node, const std::string &dependent_node);

};

#endif //PIGRAMMERS_SPREADSHEET_SERVER_SERVER_DEPENDENCY_GRAPH_H
