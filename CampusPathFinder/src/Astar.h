#ifndef ASTAR_H
#define ASTAR_H
#include <cmath>
#include "Graph.h"
#include "ShortestPathBase.h"


class Astar : public ShortestPathBase {
    public:
        const double kEarthRadius = 6371000; // meters

        explicit Astar(const std::string& filename);
        PathResult computePath(const std::string& start, const std::string& end) override;
    private:
        double deg2rad(double deg);
        double haversine(double lat1, double lon1, double lat2, double lon2);
        
};

#endif
