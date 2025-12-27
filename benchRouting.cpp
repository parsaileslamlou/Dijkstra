// bench_routing.cpp
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include <cassert>

#include "CampusPathFinder/src/Graph.h"
#include "CampusPathFinder/src/Dijkstra.h"
#include "CampusPathFinder/src/Astar.h"

// Collecing node IDs from the graph so we can benchmark random pairs.
static std::vector<std::string> collectNodeIds(const Graph& g, size_t maxNodes = 5000) {
    std::vector<std::string> ids;
    ids.reserve(std::min(maxNodes, g.getList().size()));

    for (const auto& kv : g.getList()) {
        ids.push_back(kv.first);
        if (ids.size() >= maxNodes) break;
    }
    return ids;
}

// Pick random (start,end) pairs from nodeIds.
// allows repeats
static std::vector<std::pair<std::string, std::string>>
makeRandomPairs(const std::vector<std::string>& nodeIds, int trials, uint32_t seed = 42) {
    assert(nodeIds.size() >= 2);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> dist(0, nodeIds.size() - 1);

    std::vector<std::pair<std::string, std::string>> pairs;
    pairs.reserve(trials);

    for (int i = 0; i < trials; ++i) {
        std::string s = nodeIds[dist(rng)];
        std::string t = nodeIds[dist(rng)];
        while (t == s) t = nodeIds[dist(rng)];
        pairs.push_back({s, t});
    }
    return pairs;
}

struct Stats {
    int trials = 0;
    int reachable = 0;
    double totalMs = 0.0;
    double totalWeight = 0.0;
    double totalPathLen = 0.0;

    double avgMs() const { return trials ? totalMs / trials : 0.0; }
    double avgMsReachable() const { return reachable ? totalMs / reachable : 0.0; }
    double avgWeightReachable() const { return reachable ? totalWeight / reachable : 0.0; }
    double avgPathLenReachable() const { return reachable ? totalPathLen / reachable : 0.0; }
};

template <typename Algo>
static Stats runBench(Algo& algo,
                      const std::vector<std::pair<std::string, std::string>>& pairs) {
    Stats st;
    st.trials = (int)pairs.size();

    for (const auto& [s, t] : pairs) {
        auto res = algo.computePath(s, t);
        st.totalMs += res.elapsedMs;

        if (res.reachable) {
            st.reachable++;
            st.totalWeight += res.weight;
            st.totalPathLen += (double)res.path.size();
        }
    }
    return st;
}

static void printStats(const std::string& name, const Stats& st) {
    std::cout << "\n== " << name << " ==\n";
    std::cout << "Trials: " << st.trials << "\n";
    std::cout << "Reachable: " << st.reachable << " ("
              << std::fixed << std::setprecision(1)
              << (st.trials ? (100.0 * st.reachable / st.trials) : 0.0) << "%)\n";

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Avg time (all):       " << st.avgMs() << " ms\n";
    std::cout << "Avg time (reachable): " << st.avgMsReachable() << " ms\n";

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Avg weight (reachable):   " << st.avgWeightReachable() << "\n";
    std::cout << "Avg path nodes (reachable): " << st.avgPathLenReachable() << "\n";
}

int main() {
    const std::string file = "data/ucla.geojson";

    // Load graph once just to collect node IDs for random sampling.
    Graph g;
    g.loadFromFile(file);

    auto nodeIds = collectNodeIds(g, 4000);
    std::cout << "Collected node IDs: " << nodeIds.size() << "\n";
    if (nodeIds.size() < 2) {
        std::cerr << "Not enough nodes to benchmark.\n";
        return 1;
    }

    const int trials = 5000;       // increase later (e.g. 1000) once everything is stable
    const uint32_t seed = 42;

    auto pairs = makeRandomPairs(nodeIds, trials, seed);

    // Instantiate algorithms (they load the same graph file internally)
    Dijkstra dij(file);
    Astar ast(file);

    // Run benchmarks
    auto dijStats = runBench(dij, pairs);
    auto astStats = runBench(ast, pairs);

    printStats("Dijkstra", dijStats);
    printStats("A*", astStats);

    // Simple speedup report
    std::cout << "\n== Speedup ==\n";
    if (astStats.avgMs() > 0.0) {
        double speedup = dijStats.avgMs() / astStats.avgMs();
        std::cout << std::fixed << std::setprecision(2)
                  << "Dijkstra / A* avg time speedup: " << speedup << "x\n";
    } else {
        std::cout << "A* avg time is 0 (unexpected). Increase trials or check timing.\n";
    }

    return 0;
}
