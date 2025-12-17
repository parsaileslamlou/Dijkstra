# Campus Path Finder Engine using Dijkstra's Algorithm

**Overview / Summary**

This project implements a **shortest path finding engine** for a campus map using **Dijkstra's Algorithm** and **advanced C++ data structures**. The system models the campus as a weighted, directed graph, allowing for efficient route calculations.

Key concepts and sophisticated techniques demonstrated:

- **Dijkstra's Algorithm** implemented using a **Min-Priority Queue** (`std::priority_queue` with `std::greater`) for $O(E \log V)$ average time complexity.
- **Graph Representation** using an **Adjacency List** (`std::unordered_map`) for fast neighbor lookup.
- **Custom Weighting/Parsing Logic** to adjust edge weights based on terrain (`STAIRS_UP`, `STAIRS_DOWN`, `FLAT`) from a text file input.
- **Robust I/O and Error Handling** including file parsing and catching paths that don't exist.
- **Standard Library Usage**: Effective use of containers like `std::unordered_map`, `std::priority_queue`, `std::vector`, and `std::stack`.

This system is suitable for navigation apps, route optimization, or understanding graph theory concepts.

---

## Dijkstra Module

Implements the shortest path finding algorithm.

**Core functionality:**
- **Initialization**: Sets all distances to $\infty$ and predecessors to null.
- **Path Calculation**: Uses a Min-Priority Queue to iteratively find the shortest path from a starting node to all reachable nodes.
- **Predecessor Tracking**: Stores the `prev` map to reconstruct the path after the algorithm finishes.
- **Output**: Prints the resulting path (Start -> ... -> End) and the total weight.

Ensures the most optimal route is found based on the defined edge weights.

---

## Graph Module

Handles the loading and storage of the campus map data.

**Features:**
- **Parsing**: Reads edge data (Start Node, End Node, Base Weight, Terrain) from `data/campus_graph.txt`.
- **Weight Adjustment**: Applies multipliers to the base weight based on terrain type (`STAIRS_UP`: $\times 1.8$, `STAIRS_DOWN`: $\times 1.2$).
- **Storage**: Uses `std::unordered_map<std::string, std::vector<std::pair<std::string, double>>>` to store the directed adjacency list.
- **Access**: Provides `getNeighbors()` for efficient traversal during pathfinding.

Used for maintaining the structured data representation of the campus.

---

## File Format 

The graph data is read from the `data/campus_graph.txt` file. Each line in the file defines a single directed edge and follows this format:
[START_NODE] [END_NODE] [BASE_WEIGHT] [TERRAIN_TYPE]

