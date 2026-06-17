#ifndef MENU_H
#define MENU_H

#include "Graph.h"

// Lớp Menu xử lý giao diện tương tác dòng lệnh (CLI) cho người dùng
class Menu {
public:
    // Hiển thị danh sách menu lựa chọn
    void displayMenu();

    // Vòng lặp chính của ứng dụng
    void run(Graph& graph);

private:
    // Quản lý User
    void handleAddUser(Graph& graph);
    void handleRemoveUser(Graph& graph);

    // Quản lý Friendship
    void handleAddFriendship(Graph& graph);
    void handleRemoveFriendship(Graph& graph);

    // Truy vấn thông tin
    void handleFriendCount(const Graph& graph);
    void handleMutualFriends(const Graph& graph);

    // Thuật toán
    void handleBFS(const Graph& graph);
    void handleShortestPath(const Graph& graph);
    void handleFriendSuggestions(const Graph& graph);

    // Hiển thị & Trực quan hóa
    void handleDisplayNetwork(const Graph& graph);
    void handleVisualize(const Graph& graph);
};

#endif // MENU_H
