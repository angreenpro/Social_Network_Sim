#include "Graph.h"
#include "Algorithms.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;

// Helper: Generate a random graph with specified number of vertices and edges
Graph generateRandomGraph(int numUsers, int numEdges) {
    Graph g;
    // Add users
    for (int i = 1; i <= numUsers; ++i) {
        g.addUser(i, "User" + to_string(i), 20 + (i % 30), "City" + to_string(i % 10));
    }

    // Add edges
    mt19937 rng(42); // Fixed seed for reproducibility
    uniform_int_distribution<int> dist(1, numUsers);

    int edgesAdded = 0;
    while (edgesAdded < numEdges) {
        int u = dist(rng);
        int v = dist(rng);
        if (u != v && !g.hasEdge(u, v)) {
            g.addEdge(u, v);
            edgesAdded++;
        }
    }
    return g;
}

// Helper: Run algorithms and measure time
void testPerformance(int V, int E) {
    cout << "--------------------------------------------------------\n";
    cout << "Generating graph with V = " << V << ", E = " << E << "...\n";
    
    auto startGen = high_resolution_clock::now();
    Graph g = generateRandomGraph(V, E);
    auto endGen = high_resolution_clock::now();
    cout << "Graph generation took: " 
         << duration_cast<milliseconds>(endGen - startGen).count() << " ms\n";

    // 1. BFS Traversal
    int startNode = 1;
    auto startBfs = high_resolution_clock::now();
    vector<int> bfsOrder = Algorithms::bfsTraversal(g, startNode);
    auto endBfs = high_resolution_clock::now();
    cout << "BFS Traversal O(V+E) took: " 
         << duration_cast<milliseconds>(endBfs - startBfs).count() << " ms "
         << "(visited " << bfsOrder.size() << " nodes)\n";

    // 2. Mutual Friends
    int u = 1, v = 2;
    auto startMutual = high_resolution_clock::now();
    vector<int> mutual = Algorithms::getMutualFriends(g, u, v);
    auto endMutual = high_resolution_clock::now();
    cout << "Mutual Friends O(min(deg(u), deg(v))) took: " 
         << duration_cast<microseconds>(endMutual - startMutual).count() << " microseconds "
         << "(found " << mutual.size() << " mutual friends)\n";

    // 3. Friend Suggestions
    int suggestFor = 1;
    auto startSuggest = high_resolution_clock::now();
    auto suggestions = Algorithms::friendSuggestions(g, suggestFor, 10);
    auto endSuggest = high_resolution_clock::now();
    cout << "Friend Suggestions O(deg^2 + k log k) took: " 
         << duration_cast<milliseconds>(endSuggest - startSuggest).count() << " ms "
         << "(found " << suggestions.size() << " suggestions)\n";

    // 4. Shortest Path
    int targetNode = V / 2;
    auto startPath = high_resolution_clock::now();
    vector<int> path = Algorithms::shortestPath(g, startNode, targetNode);
    auto endPath = high_resolution_clock::now();
    cout << "Shortest Path O(V+E) took: " 
         << duration_cast<milliseconds>(endPath - startPath).count() << " ms "
         << "(path length " << (path.empty() ? 0 : path.size() - 1) << ")\n";
}

int main() {
    cout << "=== PERFORMANCE TESTS ===\n\n";
    
    // Scale 1: V = 10,000, E = 50,000 (Average degree = 10)
    testPerformance(10000, 50000);

    // Scale 2: V = 50,000, E = 500,000 (Average degree = 20)
    testPerformance(50000, 500000);

    // Scale 3: V = 100,000, E = 2,000,000 (Average degree = 40)
    testPerformance(100000, 2000000);

    cout << "--------------------------------------------------------\n";
    cout << "Performance tests completed successfully.\n";
    return 0;
}
