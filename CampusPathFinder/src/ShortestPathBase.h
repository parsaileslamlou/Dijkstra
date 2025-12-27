#ifndef SHORTESTPATHBASE_H
#define SHORTESTPATHBASE_H

#include <chrono>
#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>
#include <utility>
#include "Graph.h"

class ShortestPathBase {
    public:
        struct PathResult{
            bool reachable;
            std::vector<std::string> path;
            double weight;
            double elapsedMs;   
        };
        virtual PathResult computePath(const std::string& start, const std::string& end) = 0;
    
    protected:
        void Init();
        void printPath(const std::string& start, const std::string& end) const;
    
        Graph graph;
        std::unordered_map<std::string, std::string> prev;
        std::unordered_map<std::string, bool> visited;
        std::unordered_map<std::string, double> dist;

        using PQItem = std::pair<double, std::string>;
        std::priority_queue<
            PQItem,
            std::vector<PQItem>,
            std::greater<PQItem>
        > pq;
};
#endif
