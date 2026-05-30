#ifndef MENU_H
#define MENU_H

#include "Graph.h"

// =============================================================================
// Class Menu
// Giao diện tương tác dòng lệnh (CLI) — điểm tiếp xúc giữa user và hệ thống.
//
// Menu là ORCHESTRATOR — kết nối tất cả module lại với nhau:
//   - Hiển thị menu lựa chọn
//   - Đọc input từ bàn phím
//   - Validate input
//   - Dispatch đến hàm xử lý tương ứng (Graph, Algorithms, CSVHandler, Visualizer)
//   - Hiển thị kết quả
//
// MENU OPTIONS:
//   [1]  Thêm người dùng           → addUser()
//   [2]  Xóa người dùng            → removeUser() + rewrite CSVs
//   [3]  Thêm mối quan hệ bạn bè   → addEdge() + append CSV
//   [4]  Xóa mối quan hệ bạn bè    → removeEdge() + rewrite CSV
//   [5]  Xem số lượng bạn bè       → getDegree()
//   [6]  Xem danh sách bạn chung   → getMutualFriends()
//   [7]  BFS Traversal             → bfsTraversal()
//   [8]  Tìm đường đi ngắn nhất   → shortestPath()
//   [9]  Hiển thị mạng lưới       → displayNetwork()
//   [10] Gợi ý kết bạn            → friendSuggestions()
//   [11] Trực quan hóa đồ thị     → Visualizer::drawGraph()
//   [0]  Thoát                    → save & exit
// =============================================================================
class Menu {
public:
    // -------------------------------------------------------------------------
    // displayMenu()
    // In danh sách menu lựa chọn ra stdout.
    // -------------------------------------------------------------------------
    void displayMenu();

    // -------------------------------------------------------------------------
    // run()
    // Vòng lặp chính của ứng dụng:
    //   1. Hiển thị menu
    //   2. Đọc lựa chọn của user
    //   3. Dispatch đến handler tương ứng
    //   4. Lặp lại cho đến khi user chọn Exit (0)
    // Bao gồm input validation và error handling.
    // -------------------------------------------------------------------------
    void run(Graph& graph);

private:
    // =========================================================================
    // HANDLERS — Mỗi handler xử lý một chức năng cụ thể
    // Workflow: prompt input → validate → gọi hàm xử lý → hiển thị kết quả
    // =========================================================================

    // -------------------------------------------------------------------------
    // Quản lý User
    // -------------------------------------------------------------------------
    void handleAddUser(Graph& graph);           // Nhập id, name, age, location → thêm user
    void handleRemoveUser(Graph& graph);        // Nhập id → xóa user + rewrite CSV

    // -------------------------------------------------------------------------
    // Quản lý Friendship
    // -------------------------------------------------------------------------
    void handleAddFriendship(Graph& graph);     // Nhập 2 ID → thêm edge + append CSV
    void handleRemoveFriendship(Graph& graph);  // Nhập 2 ID → xóa edge + rewrite CSV

    // -------------------------------------------------------------------------
    // Truy vấn thông tin
    // -------------------------------------------------------------------------
    void handleFriendCount(const Graph& graph);      // Nhập ID → hiển thị số bạn bè
    void handleMutualFriends(const Graph& graph);    // Nhập 2 ID → hiển thị danh sách bạn chung

    // -------------------------------------------------------------------------
    // Thuật toán
    // -------------------------------------------------------------------------
    void handleBFS(const Graph& graph);              // Nhập start ID → duyệt BFS, in thứ tự
    void handleShortestPath(const Graph& graph);     // Nhập source & target → tìm shortest path
    void handleFriendSuggestions(const Graph& graph); // Nhập ID + topK → gợi ý kết bạn

    // -------------------------------------------------------------------------
    // Hiển thị & Trực quan hóa
    // -------------------------------------------------------------------------
    void handleDisplayNetwork(const Graph& graph);   // In toàn bộ mạng lưới (adjacency list)
    void handleVisualize(const Graph& graph);        // Gọi Visualizer để vẽ đồ thị
};

#endif // MENU_H
