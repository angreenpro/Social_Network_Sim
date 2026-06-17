#include "Graph.h"
#include <iostream>
#include <cassert>

// =============================================================================
// Unit Tests cho Graph Module
// Sử dụng assert() — test fail sẽ terminate chương trình với thông báo lỗi.
// =============================================================================

void testAddAndHasUser() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(2, "Bob", 25, "Ho Chi Minh");

    assert(g.hasUser(1));
    assert(g.hasUser(2));
    assert(!g.hasUser(3));  // User không tồn tại
    assert(g.getUserCount() == 2);

    std::cout << "  [PASS] testAddAndHasUser\n";
}

void testGetUser() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");

    User u = g.getUser(1);
    assert(u.getId() == 1);
    assert(u.getName() == "Alice");
    assert(u.getAge() == 22);
    assert(u.getLocation() == "Ha Noi");

    std::cout << "  [PASS] testGetUser\n";
}

void testGetAllUserIds() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(5, "Bob", 25, "HCM");
    g.addUser(10, "Charlie", 21, "Da Nang");

    std::vector<int> ids = g.getAllUserIds();
    assert(ids.size() == 3);
    // Kiểm tra tất cả ID đều có mặt (thứ tự không quan trọng vì unordered_map)
    bool has1 = false, has5 = false, has10 = false;
    for (int id : ids) {
        if (id == 1) has1 = true;
        if (id == 5) has5 = true;
        if (id == 10) has10 = true;
    }
    assert(has1 && has5 && has10);

    std::cout << "  [PASS] testGetAllUserIds\n";
}

void testAddAndHasEdge() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(2, "Bob", 25, "Ho Chi Minh");
    g.addUser(3, "Charlie", 21, "Da Nang");

    g.addEdge(1, 2);
    assert(g.hasEdge(1, 2));
    assert(g.hasEdge(2, 1));  // Undirected → cả 2 chiều
    assert(!g.hasEdge(1, 3)); // Chưa kết nối

    std::cout << "  [PASS] testAddAndHasEdge\n";
}

void testRemoveEdge() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(2, "Bob", 25, "HCM");

    g.addEdge(1, 2);
    assert(g.hasEdge(1, 2));

    g.removeEdge(1, 2);
    assert(!g.hasEdge(1, 2));
    assert(!g.hasEdge(2, 1));

    std::cout << "  [PASS] testRemoveEdge\n";
}

void testRemoveUser() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(2, "Bob", 25, "HCM");
    g.addUser(3, "Charlie", 21, "Da Nang");

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);

    // Xóa user 1 — phải xóa cả edges 1-2, 1-3
    g.removeUser(1);
    assert(!g.hasUser(1));
    assert(!g.hasEdge(1, 2));
    assert(!g.hasEdge(1, 3));
    assert(g.hasEdge(2, 3));  // Edge 2-3 vẫn tồn tại
    assert(g.getUserCount() == 2);

    std::cout << "  [PASS] testRemoveUser\n";
}

void testGetNeighborsAndDegree() {
    Graph g;
    g.addUser(1, "Alice", 22, "Ha Noi");
    g.addUser(2, "Bob", 25, "HCM");
    g.addUser(3, "Charlie", 21, "Da Nang");
    g.addUser(4, "David", 28, "Hue");

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);

    assert(g.getDegree(1) == 3);
    assert(g.getDegree(2) == 1);

    auto neighbors = g.getNeighbors(1);
    assert(neighbors.size() == 3);
    assert(neighbors.count(2) == 1);
    assert(neighbors.count(3) == 1);
    assert(neighbors.count(4) == 1);

    std::cout << "  [PASS] testGetNeighborsAndDegree\n";
}

void testEdgeCount() {
    Graph g;
    g.addUser(1, "A", 20, "HN");
    g.addUser(2, "B", 20, "HN");
    g.addUser(3, "C", 20, "HN");

    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(1, 3);

    assert(g.getEdgeCount() == 3);

    // Duplicate edge — không nên tăng edge count
    g.addEdge(1, 2);
    assert(g.getEdgeCount() == 3);

    std::cout << "  [PASS] testEdgeCount\n";
}

void testEmptyNeighbors() {
    Graph g;
    g.addUser(1, "Alice", 22, "HN");

    // User không có bạn bè
    assert(g.getDegree(1) == 0);
    assert(g.getNeighbors(1).empty());

    // User không tồn tại
    assert(g.getDegree(999) == 0);
    assert(g.getNeighbors(999).empty());

    std::cout << "  [PASS] testEmptyNeighbors\n";
}

// =============================================================================
// Main — chạy tất cả tests
// =============================================================================
int main() {
    std::cout << "\n=== GRAPH UNIT TESTS ===\n\n";

    testAddAndHasUser();
    testGetUser();
    testGetAllUserIds();
    testAddAndHasEdge();
    testRemoveEdge();
    testRemoveUser();
    testGetNeighborsAndDegree();
    testEdgeCount();
    testEmptyNeighbors();

    std::cout << "\n=== ALL GRAPH TESTS PASSED ===\n\n";
    return 0;
}
