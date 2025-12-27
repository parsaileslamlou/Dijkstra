#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

class ShortestPathBase; // so we can use nodeData

class Graph {
    public:
        const double kEarthRadius = 6371000; // meters
        struct NodeLocation {
            double lat;
            double lon;
            std::string name;
        };
        Graph() = default;
    
        void loadFromFile(const std::string& filename);
        bool containsNode(const std::string& node) const;

        NodeLocation getNodeLocation(const std::string& nodeId) const;
        std::vector<std::pair<std::string, double>> getNeighbors(const std::string& node) const;
        std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> getList() const;
   
    private:
        friend class ShortestPathBase;
        
        double deg2rad(double deg);
        double haversine(double lat1, double lon1, double lat2, double lon2);

        void parseFile(const std::string& filename);
       
        std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> adjList;
        std::unordered_map<std::string, NodeLocation> nodeData;
        // Key is the starting Node, and value is an array of Nodes to visit with the weight
};
#endif
