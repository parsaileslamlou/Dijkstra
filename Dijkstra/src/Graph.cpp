#include "Graph.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

Graph::Graph(const std::string& filename){
    parseFile(filename);
}

std::vector<std::pair<std::string, double>> getNeighbors(const std::string& node) const{
    auto it = adjList.find(node);
    if (it != adjList.end()) return it->second;
    return {}; // return empty vector if not found
}

void parseFile(const std::string& filename){
    ifstream inFile;
    inFile.open("data/campus_graph.txt");    
}



