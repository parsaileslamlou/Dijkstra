#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

class Graph {
public:
    Graph(const std::string& filename);
    std::vector<std::pair<std::string, double>> getNeighbors(const std::string& node) const;
private:
    void parseFile(const std::string& filename);
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> adjList;
    // Key is the starting Node, and value is an array of Nodes to visit with the weight
};
#endif
