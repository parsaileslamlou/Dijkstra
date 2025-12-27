#include <string>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <functional>
#include <unordered_map>
#include <utility>
#include <limits>
#include "ShortestPathBase.h"

void ShortestPathBase::Init(){ //Intializes all helper containers to default friendly values
    prev.clear();
    visited.clear();
    dist.clear();
    
    for (auto& kv : graph.nodeData) {
        prev[kv.first] = "";
        visited[kv.first] = false;
        dist[kv.first] = std::numeric_limits<double>::infinity();
    }
    
    while (!pq.empty()) pq.pop();

}

void ShortestPathBase::printPath(const std::string& start, const std::string& end) const{ //helper to print best path
    std::string traversal = end;
    std::stack<std::string> path;
    
    while(traversal != start){
        path.push(traversal);
        traversal = prev.at(traversal);
    }
    path.push(start);
    
    while(!path.empty()){
        std::cout << path.top() << " ";
        path.pop();
    }
}
