#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Graph.h"
#include "ShortestPathBase.h"

class Dijkstra : public ShortestPathBase {
    public:
        explicit Dijkstra(const std::string& filename);
         PathResult computePath(const std::string& start, const std::string& end) override;
};

#endif
