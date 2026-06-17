#include "Graph.h"
#include "Algorithms.h"
#include <iostream>
#include <cassert>
#include <algorithm>

// =============================================================================
// Helper — tạo graph mẫu cho testing
//
//   Alice(1) --- Bob(2) --- David(4)
//     |            |          |
//   Charlie(3) ---+--- Eve(5)
//
// Edges: 1-2, 1-3, 2-3, 2-4, 4-5, 3-5
// =============================================================================
Graph createTestGraph() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(2, "Bob", 25, "Ho Chi Minh");
    g.addUser(3, "Charlie", 21, "Da Nang");
    g.addUser(4, "David", 28, "Hue");
    g.addUser(5, "Eve", 24, "Can Tho");

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 5);
    g.addEdge(3, 5);

    return g;
}

// =============================================================================
// Test BFS Traversal
// =============================================================================
void testBFS() {
    Graph g = createTestGraph();

    std::vector<int> order = Algorithms::bfsTraversal(g, 1);

    // BFS từ 1 phải thăm tất cả 5 nodes
    assert(order.size() == 5);

    // Node đầu tiên phải là start node
    assert(order[0] == 1);

    // Tất cả nodes phải xuất hiện đúng 1 lần
    std::vector<int> sorted = order;
    std::sort(sorted.begin(), sorted.end());
    assert(sorted == std::vector<int>({1, 2, 3, 4, 5}));

    // BFS từ node không tồn tại → rỗng
    std::vector<int> empty = Algorithms::bfsTraversal(g, 999);
    assert(empty.empty());

    std::cout << "  [PASS] testBFS\n";
}

// =============================================================================
// Test BFS trên disconnected graph
// =============================================================================
void testBFSDisconnected() {
    Graph g;
    g.addUser(1, "A", 20, "HN");
    g.addUser(2, "B", 20, "HN");
    g.addUser(3, "C", 20, "HN");
    g.addUser(4, "D", 20, "HN");

    g.addEdge(1, 2);
    // 3 và 4 isolated

    std::vector<int> order = Algorithms::bfsTraversal(g, 1);
    // Chỉ thăm được component chứa node 1 → {1, 2}
    assert(order.size() == 2);
    assert(order[0] == 1);
    assert(order[1] == 2);

    std::cout << "  [PASS] testBFSDisconnected\n";
}

// =============================================================================
// Test Mutual Friends
// =============================================================================
void testMutualFriends() {
    Graph g = createTestGraph();

    // Alice(1) và David(4): bạn chung = Bob(2)
    // Vì: Alice friends = {2, 3}, David friends = {2, 5}
    // Intersection = {2}
    std::vector<int> mutual14 = Algorithms::getMutualFriends(g, 1, 4);
    assert(mutual14.size() == 1);
    assert(mutual14[0] == 2);

    // Alice(1) và Eve(5): bạn chung = Charlie(3)
    // Vì: Alice friends = {2, 3}, Eve friends = {4, 3}
    // Intersection = {3}
    std::vector<int> mutual15 = Algorithms::getMutualFriends(g, 1, 5);
    assert(mutual15.size() == 1);
    assert(mutual15[0] == 3);

    // Bob(2) và Eve(5): bạn chung = Charlie(3), David(4)
    std::vector<int> mutual25 = Algorithms::getMutualFriends(g, 2, 5);
    assert(mutual25.size() == 2);

    // Mutual friends với user không tồn tại → rỗng
    std::vector<int> mutualInvalid = Algorithms::getMutualFriends(g, 1, 999);
    assert(mutualInvalid.empty());

    std::cout << "  [PASS] testMutualFriends\n";
}

// =============================================================================
// Test Friend Suggestions
// =============================================================================
void testFriendSuggestions() {
    Graph g = createTestGraph();

    // Gợi ý cho Alice(1):
    //   Direct friends = {2, 3}
    //   Bob(2) friends = {1, 3, 4} → candidate: 4
    //   Charlie(3) friends = {1, 2, 5} → candidate: 5
    //   Candidate 4 (David): mutual with Alice = {2} → score 1
    //                        + xuất hiện qua Bob → score count = 1
    //   Candidate 5 (Eve): mutual with Alice = {3} → score 1
    //                      + xuất hiện qua Charlie → score count = 1
    auto suggestions = Algorithms::friendSuggestions(g, 1, 5);
    assert(suggestions.size() == 2);

    // Cả 2 candidate đều có score = 1 mutual friend
    assert(suggestions[0].second == 1);
    assert(suggestions[1].second == 1);

    // Vì score bằng nhau, sort theo ID tăng dần → 4 trước 5
    assert(suggestions[0].first == 4);
    assert(suggestions[1].first == 5);

    // Top-1 chỉ trả về 1 kết quả
    auto top1 = Algorithms::friendSuggestions(g, 1, 1);
    assert(top1.size() == 1);

    // User không tồn tại → rỗng
    auto invalid = Algorithms::friendSuggestions(g, 999, 5);
    assert(invalid.empty());

    std::cout << "  [PASS] testFriendSuggestions\n";
}

// =============================================================================
// Test Friend Suggestions — complex graph (higher mutual counts)
// =============================================================================
void testFriendSuggestionsComplex() {
    Graph g;
    // Hub graph: user 1 connected to 2,3,4,5
    // User 6 connected to 2,3,4 (3 mutual friends with 1)
    // User 7 connected to 2,3 (2 mutual friends with 1)
    // User 8 connected to 5 (1 mutual friend with 1)
    g.addUser(1, "Center", 20, "HN");
    for (int i = 2; i <= 8; i++)
        g.addUser(i, "User" + std::to_string(i), 20, "HN");

    g.addEdge(1, 2); g.addEdge(1, 3); g.addEdge(1, 4); g.addEdge(1, 5);
    g.addEdge(6, 2); g.addEdge(6, 3); g.addEdge(6, 4);
    g.addEdge(7, 2); g.addEdge(7, 3);
    g.addEdge(8, 5);

    auto suggestions = Algorithms::friendSuggestions(g, 1, 10);
    assert(suggestions.size() == 3);  // 6, 7, 8

    // Sorted by mutual count descending
    assert(suggestions[0].first == 6 && suggestions[0].second == 3);
    assert(suggestions[1].first == 7 && suggestions[1].second == 2);
    assert(suggestions[2].first == 8 && suggestions[2].second == 1);

    std::cout << "  [PASS] testFriendSuggestionsComplex\n";
}

// =============================================================================
// Test Shortest Path
// =============================================================================
void testShortestPath() {
    Graph g = createTestGraph();

    // Alice(1) → David(4): path = 1 → 2 → 4 (length 2)
    std::vector<int> path14 = Algorithms::shortestPath(g, 1, 4);
    assert(path14.size() == 3);
    assert(path14[0] == 1);
    assert(path14[1] == 2);
    assert(path14[2] == 4);

    // Alice(1) → Eve(5): path = 1 → 3 → 5 (length 2)
    std::vector<int> path15 = Algorithms::shortestPath(g, 1, 5);
    assert(path15.size() == 3);
    assert(path15[0] == 1);
    assert(path15[2] == 5);

    // Same node → [1]
    std::vector<int> same = Algorithms::shortestPath(g, 1, 1);
    assert(same.size() == 1);
    assert(same[0] == 1);

    // Node không tồn tại → rỗng
    std::vector<int> invalid = Algorithms::shortestPath(g, 1, 999);
    assert(invalid.empty());

    std::cout << "  [PASS] testShortestPath\n";
}

// =============================================================================
// Test Shortest Path — no path exists (disconnected graph)
// =============================================================================
void testShortestPathNoRoute() {
    Graph g;
    g.addUser(1, "A", 20, "HN");
    g.addUser(2, "B", 20, "HN");
    g.addUser(3, "C", 20, "HN");

    g.addEdge(1, 2);
    // 3 is isolated → no path 1→3

    std::vector<int> noPath = Algorithms::shortestPath(g, 1, 3);
    assert(noPath.empty());

    std::cout << "  [PASS] testShortestPathNoRoute\n";
}

// =============================================================================
// Main — chạy tất cả tests
// =============================================================================
int main() {
    std::cout << "\n=== ALGORITHMS UNIT TESTS ===\n\n";

    testBFS();
    testBFSDisconnected();
    testMutualFriends();
    testFriendSuggestions();
    testFriendSuggestionsComplex();
    testShortestPath();
    testShortestPathNoRoute();

    std::cout << "\n=== ALL ALGORITHM TESTS PASSED ===\n\n";
    return 0;
}
