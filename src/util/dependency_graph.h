#ifndef SERVER_DEPENDENCY_GRAPH_H
#define SERVER_DEPENDENCY_GRAPH_H

#include <string>
#include <map>
#include <set>

namespace spreadsheet {

    /**
     * A Dependency Graph stores relational mappings between objects with the assumption that
     * one object depends in some way on another.
     *
     * This particular dependency graph stores strings.
     *
     * For example, in the case of a spreadsheet, one cell may depend on the value of another cell,
     * in which case the latter cell must be evaluated before the former.
     */
    class DependencyGraph {

    private:

        /**
         * Maps a node to a set of nodes which depend upon it.
         */
        std::map <std::string, std::set<std::string>> dependents;

        /**
         * Maps a node to a set of nodes which it depends upon.
         */
        std::map <std::string, std::set<std::string>> dependees;

        /**
         * The number of pairs in the graph.
         */
        int size;

    public:

        /**
         * @return The number of ordered pairs in the graph.
         */
        int size();

        /**
         * Finds all dependents of the given node.
         * @param node The node.
         * @return Dependents of the node (Those nodes which depend on the given node).
         */
        std::set <std::string> getDependents(std::string node);

        /**
         * Finds all dependees of the given node.
         * @param node The node.
         * @return Dependees of the node (Those nodes which the given node depends on).
         */
        std::set <std::string> getDependees(std::string node);

        /**
         * Adds a dependency based on the given nodes.
         * @param node The reference node.
         * @param dependentNode The node that depends upon the reference node.
         */
        void addDependency(std::string node, std::string dependentNode);

        /**
         * Removes a dependency based on the given nodes.
         * @param node The reference node.
         * @param dependentNode The node that depends on the reference node.
         */
        void removeDependency(std::string node, std::string dependentNode);

    };

};

#endif //SERVER_DEPENDENCY_GRAPH_H
