// test_routing.cpp
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "CampusPathFinder/src/Dijkstra.h"
#include "CampusPathFinder/src/Graph.h"

// Helper: pick two valid nodes from the loaded graph.
static std::pair<std::string, std::string> pickTwoNodes(const Graph& g) {
    auto list = g.getList();
    assert(!list.empty());

    std::string start, end;

    // start: first node that has at least 1 neighbor
    for (const auto& kv : list) {
        if (!kv.second.empty()) {
            start = kv.first;
            break;
        }
    }
    assert(!start.empty());

    // end: pick a neighbor of start (guarantees reachable in an undirected segment)
    end = list[start].front().first;
    assert(!end.empty());

    return {start, end};
}

static void testReachableSimple() {
    Graph g;
    g.loadFromFile("data/ucla.geojson");

    auto [start, end] = pickTwoNodes(g);

    Dijkstra d("data/ucla.geojson");
    auto res = d.computePath(start, end);

    assert(res.reachable == true);
    assert(!res.path.empty());
    assert(res.path.front() == start);
    assert(res.path.back() == end);

    // With your current parser weights=1.0 per edge, this should be at least 1.0
    assert(res.weight >= 1.0);

    // Timing should be non-negative
    assert(res.elapsedMs >= 0.0);

    std::cout << "PASS: testReachableSimple\n";
}

static void testSameStartEnd() {
    Graph g;
    g.loadFromFile("data/ucla.geojson");

    // pick any valid node
    std::string node;
    for (const auto& kv : g.getList()) {
        node = kv.first;
        break;
    }
    assert(!node.empty());

    Dijkstra d("data/ucla.geojson");
    auto res = d.computePath(node, node);

   
    assert(res.reachable == true);
    assert(res.path.size() == 1);
    assert(res.path[0] == node);
    assert(res.weight == 0.0);
    assert(res.elapsedMs >= 0.0);

    std::cout << "PASS: testSameStartEnd\n";
}

static void testInvalidNodeNames() {
    Dijkstra d("data/ucla.geojson");

    auto res = d.computePath("NOT_A_REAL_NODE", "ALSO_NOT_REAL");
    assert(res.reachable == false);
    assert(res.path.empty());
    assert(res.weight == 0.0);
    assert(res.elapsedMs >= 0.0);

    std::cout << "PASS: testInvalidNodeNames\n";
}

static void testPathIsContinuousEdges() {
    Graph g;
    g.loadFromFile("data/ucla.geojson");

    auto [start, end] = pickTwoNodes(g);

    Dijkstra d("data/ucla.geojson");
    auto res = d.computePath(start, end);

    assert(res.reachable);
    assert(res.path.size() >= 2);

    // Verify each consecutive step is an actual adjacency edge in the graph
    for (size_t i = 0; i + 1 < res.path.size(); ++i) {
        const std::string& u = res.path[i];
        const std::string& v = res.path[i + 1];

        auto nbrs = g.getNeighbors(u);
        bool found = false;
        for (const auto& [to, w] : nbrs) {
            if (to == v) {
                found = true;
                break;
            }
        }
        assert(found && "Path contains a non-edge hop!");
    }

    std::cout << "PASS: testPathIsContinuousEdges\n";
}

int main() {
    // These tests assume:
    // - Graph parses data/ucla.geojson into nodes + edges
    // - Dijkstra::computePath returns PathResult with fields:
    //   reachable, path, weight, elapsedMs

    testReachableSimple();
    testSameStartEnd();
    testInvalidNodeNames();
    testPathIsContinuousEdges();

    std::cout << "ALL ROUTING TESTS PASSED\n";
    return 0;
}

 
