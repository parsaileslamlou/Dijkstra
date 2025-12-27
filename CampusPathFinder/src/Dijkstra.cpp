#include "ShortestPathBase.h"
#include "Dijkstra.h"


Dijkstra::Dijkstra(const std::string& filename){
    graph.loadFromFile(filename);
    Init();
}

Dijkstra::PathResult Dijkstra::computePath(const std::string& start, const std::string& end){ //Using Dijkstra's algorithm to find best path
    auto startTime = std::chrono::high_resolution_clock::now();

    PathResult result;
    result.reachable = false;
    result.weight = 0.0;
    result.elapsedMs = 0.0;
    
    if (!graph.containsNode(start) || !graph.containsNode(end)) {
        return result; // edge check
    }

    Init();  //Resets member variables in the case the program has ran before
    
    dist[start] = 0;
    pq.push(PQItem{0,start});
    
    
    while(!pq.empty()){
        PQItem cur = pq.top();
        pq.pop();

        double removed_dist = cur.first;
        std::string removed = cur.second;

        if (visited[removed]) continue;
        if (removed_dist > dist[removed]) continue;

        visited[removed] = true;

        if (removed == end) break;
        
        for(auto& edge : graph.getNeighbors(removed)){
            if(visited[edge.first]) continue;
            double newDist = removed_dist + edge.second;
            if(newDist < dist[edge.first]){
                dist[edge.first] = newDist;
                prev[edge.first] = removed;
                pq.push(PQItem{newDist, edge.first});
            }
        }
    }
    
    if (dist[end] == std::numeric_limits<double>::infinity()) {
        return result; //reachable = false
    }
    
    std::vector<std::string> forwardPath;

    std::string traversal = end;

    while(traversal != start){
        forwardPath.push_back(traversal);
        traversal = prev.at(traversal);
    }
    forwardPath.push_back(start);
    
    std::reverse(forwardPath.begin(),forwardPath.end());
    result.reachable = true;
    result.path = forwardPath;
    result.weight = dist[end];
    
    auto endTime = std::chrono::high_resolution_clock::now();
    result.elapsedMs =
        std::chrono::duration<double, std::milli>(endTime - startTime).count();
    return result;
}



