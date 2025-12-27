# UCLA Campus Path Finder Engine — Dijkstra + A*

**Overview / Summary**

This project implements a **campus routing engine** in **C++** that finds shortest walking paths on the UCLA campus using real **OpenStreetMap (OSM) GeoJSON** data. The system parses OSM geometry into a weighted graph and supports both **Dijkstra’s Algorithm** and **A\*** search for efficient route computation.

Key concepts and techniques demonstrated:

- **Dijkstra’s Algorithm** using a **Min-Priority Queue** (`std::priority_queue` + `std::greater`) for **O(E log V)** time complexity.
- **A\*** search with a **Haversine (great-circle) heuristic** derived from node latitude/longitude for faster shortest-path queries.
- **Graph Representation** using an **Adjacency List** (`std::unordered_map`) for efficient neighbor lookup.
- **Real-World Data Parsing**: Converts OSM GeoJSON **LineString / MultiLineString** geometries into nodes + edges.
- **Meter-Accurate Edge Weights**: Edge weights are computed as geographic distances (meters) using the **Haversine** formula.
- **Robust Handling of Unreachable Routes**: Safely returns a non-reachable result when nodes are disconnected in the map data.
- **Benchmarking + Testing**: Includes benchmark harness and unit tests to validate correctness and measure performance.

This project is suitable for navigation/routing systems, route optimization, and demonstrating applied graph algorithms on real geographic data.

---

## Algorithms Module

Implements shortest-path search on the campus graph.

### Dijkstra Module

**Core functionality:**
- **Initialization**: Sets distances to \\(\\infty\\), predecessors to empty, and visited flags to false.
- **Relaxation Loop**: Uses a Min-Priority Queue to expand the currently known lowest-distance node.
- **Predecessor Tracking**: Stores a `prev` map for path reconstruction.
- **Output**: Returns a `PathResult` containing reachability, the reconstructed path, total weight (meters), and elapsed runtime.

Ensures the optimal shortest path is found for non-negative edge weights.

### A* Module

**Core functionality:**
- **Heuristic**: Uses Haversine distance to the goal as an admissible geographic heuristic.
- **Priority Ordering**: Expands nodes by \\( f(n) = g(n) + h(n) \\).
- **Same Optimality**: Produces the same shortest-path weights as Dijkstra (when reachable) while typically exploring fewer nodes and running faster.
- **Output**: Same `PathResult` structure as Dijkstra for easy comparison.

---

## Graph Module

Handles loading and representing the UCLA map as a graph.

**Features:**
- **Parsing**: Reads OSM GeoJSON from `data/ucla.geojson` (exported via Overpass Turbo / OpenStreetMap).
- **Node IDs**: Uses coordinate-based IDs formatted as `"lat,lon"` for each unique point in the geometry.
- **Edge Construction**:
  - **LineString**: Adds edges between each consecutive coordinate pair.
  - **MultiLineString**: Adds edges for each line segment group.
- **Undirected Walkways**: Adds edges in both directions (treats walk paths as bidirectional).
- **Weights**: Computes **Haversine distance** between consecutive coordinates in meters.
- **Node Metadata**: Stores latitude/longitude for use in the A* heuristic.

---

## Performance Results & Testing

Benchmarking and validation are performed using a combination of automated tests and randomized routing benchmarks to ensure correctness and performance.

### Benchmark Results
Benchmarking is performed by sampling node IDs from the graph and executing randomized routing queries.

**Sample benchmark run:**
- **Collected node IDs:** 4000  
- **Trials:** 5000  
- **Reachable pairs:** 3254 (**65.1%**)  
- **A\*** average runtime speedup vs Dijkstra: **1.41×**

**Timing (reachable routes):**
- **Dijkstra:** 9.1957 ms average  
- **A\*:** 6.5116 ms average  

**Route statistics (reachable routes):**
- **Average distance:** 1186.803 meters  
- **Average path length:** 94.019 nodes  

*Note:* Reachability is below 100% because real OpenStreetMap data contains disconnected walkway components; randomly selected node pairs may not lie in the same connected subgraph.

---

### Automated Testing

The project includes multiple test files to validate correctness and robustness:

- **`test_routing.cpp`**
  - Verifies that valid paths are found between connected nodes  
  - Confirms correct handling of invalid or unreachable node pairs  
  - Ensures path reconstruction produces contiguous, valid edges  

- **`test_graph.cpp`**
  - Validates graph construction from GeoJSON input  
  - Ensures all adjacency relationships are consistent  
  - Confirms nodes and edges are correctly populated  

All tests pass successfully, confirming correctness of graph construction, pathfinding logic, and edge-case handling across the full dataset.


## Project Structure

- `CampusPathFinder/src/`
  - `Graph.{h,cpp}` — GeoJSON parsing + graph construction
  - `Dijkstra.{h,cpp}` — Dijkstra shortest-path algorithm
  - `Astar.{h,cpp}` — A* shortest-path algorithm (Haversine heuristic)
  - `ShortestPathBase.{h,cpp}` — shared algorithm state + PathResult type
- `data/`
  - `ucla.geojson` — UCLA campus walkway data (OSM export)
- `bench_routing.cpp` — benchmark harness comparing Dijkstra vs A*
- `test_routing.cpp` — routing correctness tests (reachability, continuity, invalid nodes)
- `test_graph.cpp` — additional graph/routing tests (as included)

---

## Data Source 

The GeoJSON dataset is exported from **OpenStreetMap** using Overpass Turbo.  

---

## Project Evolution

This project began as a Dijkstra-based campus routing engine and was later extended to include:

- **A\*** search using a geographic heuristic
- A benchmark harness comparing algorithms at scale
- Expanded correctness tests for routing and graph validity
- Real UCLA OSM GeoJSON integration for realistic routing behavior
