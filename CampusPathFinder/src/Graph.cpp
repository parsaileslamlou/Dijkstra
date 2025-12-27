#include "Graph.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

void Graph::loadFromFile(const std::string& filename){
    parseFile(filename);
}

bool Graph::containsNode(const std::string& node) const {
    return nodeData.find(node) != nodeData.end();
}

Graph::NodeLocation Graph::getNodeLocation(const std::string& nodeId) const {
    auto it = nodeData.find(nodeId);
    if (it != nodeData.end()) return it->second;
    return {0.0, 0.0, ""};
}
// Key is the starting Node, and value is an array of Nodes to visit with the weight
std::vector<std::pair<std::string, double>> Graph::getNeighbors(const std::string& node) const{
    auto it = adjList.find(node);
    if (it != adjList.end()) return it->second;
    return {};
}

std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> Graph::getList() const{
    return adjList;
}

double Graph::deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// Haversine distance between two points (lat/lon in degrees)
double Graph::haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = deg2rad(lat2 - lat1);
    double dLon = deg2rad(lon2 - lon1);

    double a = std::sin(dLat/2) * std::sin(dLat/2) +
               std::cos(deg2rad(lat1)) * std::cos(deg2rad(lat2)) *
               std::sin(dLon/2) * std::sin(dLon/2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    return kEarthRadius * c;
}

void Graph::parseFile(const std::string& filename) {
    adjList.clear();
    nodeData.clear();

    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    nlohmann::json j;
    inFile >> j;

    if (!j.contains("features") || !j["features"].is_array()) {
        throw std::runtime_error("Invalid GeoJSON: missing 'features' array.");
    }

    // Helper lambda: converts a coordinate [lon, lat] -> "lat,lon"
    auto coordToId = [&](double lon, double lat) -> std::string {
        return std::to_string(lat) + "," + std::to_string(lon);
    };

    for (const auto& feature : j["features"]) {
        if (!feature.contains("geometry") || feature["geometry"].is_null()) continue;

        const auto& geom = feature["geometry"];
        if (!geom.contains("type") || !geom.contains("coordinates")) continue;

        const std::string type = geom["type"].get<std::string>();

// Case 1: LineString
        if (type == "LineString") {
            const auto& coords = geom["coordinates"];
            if (!coords.is_array() || coords.size() < 2) continue;

            for (size_t i = 0; i + 1 < coords.size(); ++i) {
                // Each coord is [lon, lat]
                double lon1 = coords[i][0].get<double>();
                double lat1 = coords[i][1].get<double>();
                double lon2 = coords[i + 1][0].get<double>();
                double lat2 = coords[i + 1][1].get<double>();

                std::string n1 = coordToId(lon1, lat1);
                std::string n2 = coordToId(lon2, lat2);

                // Store coordinates for A* heuristic later
                nodeData[n1] = {lat1, lon1, ""};
                nodeData[n2] = {lat2, lon2, ""};

                double w = haversine(lat1, lon1, lat2, lon2);

                adjList[n1].push_back({n2, w});
                adjList[n2].push_back({n1, w});
            }
        }
    
// Case 2: MultiLineString
        else if (type == "MultiLineString") {
            const auto& lines = geom["coordinates"];
            if (!lines.is_array()) continue;

            for (const auto& coords : lines) {
                if (!coords.is_array() || coords.size() < 2) continue;

                for (size_t i = 0; i + 1 < coords.size(); ++i) {
                    double lon1 = coords[i][0].get<double>();
                    double lat1 = coords[i][1].get<double>();
                    double lon2 = coords[i + 1][0].get<double>();
                    double lat2 = coords[i + 1][1].get<double>();

                    std::string n1 = coordToId(lon1, lat1);
                    std::string n2 = coordToId(lon2, lat2);

                    nodeData[n1] = {lat1, lon1, ""};
                    nodeData[n2] = {lat2, lon2, ""};

                    double w = haversine(lat1, lon1, lat2, lon2);

                    adjList[n1].push_back({n2, w});
                    adjList[n2].push_back({n1, w}); // treat as undirected walkway
                }
            }
        }

        // Ignore Polygons, MultiPolygons, Points, etc.
    }
}



