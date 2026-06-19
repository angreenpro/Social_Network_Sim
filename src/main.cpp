#include "Graph.h"
#include "CSVHandler.h"
#include "Menu.h"
#include <iostream>

int main() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "         SOCIAL NETWORK SIMULATOR v1.0                      \n";
    std::cout << "         DSA Project — Graph-based Social Network           \n";
    std::cout << "============================================================\n";

    // Khởi tạo đồ thị
    Graph graph;

    // Load dữ liệu từ file CSV
    std::cout << "  Dang tai du lieu...\n";

    CSVHandler::loadUsers("data/users.csv", graph);
    std::cout << "  [OK] Da tai " << graph.getUserCount() << " nguoi dung.\n";

    CSVHandler::loadFriendships("data/friendships.csv", graph);
    std::cout << "  [OK] Da tai " << graph.getEdgeCount() << " moi quan he.\n";

    std::cout << "============================================================\n";

    // Chạy vòng lặp menu chính
    Menu menu;
    menu.run(graph);

    return 0;
}
