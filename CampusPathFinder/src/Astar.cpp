#include "ShortestPathBase.h"
#include "Astar.h"

Astar::Astar(const std::string& filename){
     graph.loadFromFile(filename);
     Init();
 }

double Astar::deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// Haversine distance between two points (lat/lon in degrees)
double Astar::haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = deg2rad(lat2 - lat1);
    double dLon = deg2rad(lon2 - lon1);

    double a = std::sin(dLat/2) * std::sin(dLat/2) +
               std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
               std::sin(dLon/2) * std::sin(dLon/2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    return kEarthRadius * c;
}

Astar::PathResult Astar::computePath(const std::string& start, const std::string& end){
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
    
    double h0 = haversine(
        graph.getNodeLocation(start).lat,
        graph.getNodeLocation(start).lon,
        graph.getNodeLocation(end).lat,
        graph.getNodeLocation(end).lon
    );

    pq.push(PQItem{h0, start}); // f = 0 + h(start)
    
    while(!pq.empty()){
        PQItem cur = pq.top();
        pq.pop();

        std::string removed = cur.second;

        if (visited[removed]) continue;
        visited[removed] = true;

        if (removed == end) break;
        
        for(auto& edge : graph.getNeighbors(removed)){
            if(visited[edge.first]) continue;
        
            double hScore = haversine(
                graph.getNodeLocation(edge.first).lat,
                graph.getNodeLocation(edge.first).lon,
                graph.getNodeLocation(end).lat,
                graph.getNodeLocation(end).lon
            );
            double gScore = dist[removed] + edge.second;
            double fScore = gScore + hScore;
        
            if(gScore < dist[edge.first]){
                dist[edge.first] = gScore;
                prev[edge.first] = removed;
                pq.push(PQItem{fScore, edge.first});
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
