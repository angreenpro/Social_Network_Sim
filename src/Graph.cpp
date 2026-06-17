#include "Graph.h"
#include <iostream>

// Tạo User mới và thêm vào graph
void Graph::addUser(int id, const std::string& name, int age, const std::string& location) {
    users[id] = User(id, name, age, location);
    adjList[id]; // Khởi tạo empty set nếu chưa tồn tại (operator[] auto-creates)
}

// Xóa user khỏi graph và xóa các liên kết bạn bè liên quan
void Graph::removeUser(int id) {
    if (!hasUser(id)) return;

    // Xóa user khỏi adjacency set của tất cả neighbor
    for (int neighborId : adjList[id]) {
        adjList[neighborId].erase(id);
    }

    // Xóa entry của user trong adjList và users map
    adjList.erase(id);
    users.erase(id);
}

// Kiểm tra user có tồn tại trong graph không
bool Graph::hasUser(int id) const {
    return users.find(id) != users.end();
}

// Trả về User object theo ID
User Graph::getUser(int id) const {
    return users.at(id);
}

// Trả về vector chứa tất cả user ID trong graph
std::vector<int> Graph::getAllUserIds() const {
    std::vector<int> ids;
    ids.reserve(users.size()); // Pre-allocate để tránh reallocations
    for (const auto& pair : users) {
        ids.push_back(pair.first);
    }
    return ids;
}

// Trả về tổng số user trong graph
int Graph::getUserCount() const {
    return static_cast<int>(users.size());
}

// Thêm cạnh vô hướng giữa user u và user v
void Graph::addEdge(int u, int v) {
    adjList[u].insert(v);
    adjList[v].insert(u);
}

// Xóa cạnh giữa user u và user v (cả hai chiều)
void Graph::removeEdge(int u, int v) {
    adjList[u].erase(v);
    adjList[v].erase(u);
}

// Kiểm tra hai user có phải bạn bè không
bool Graph::hasEdge(int u, int v) const {
    auto it = adjList.find(u);
    if (it == adjList.end()) return false;
    return it->second.count(v) > 0;
}

// Trả về tổng số cạnh trong graph
int Graph::getEdgeCount() const {
    int totalDegree = 0;
    for (const auto& pair : adjList) {
        totalDegree += static_cast<int>(pair.second.size());
    }
    return totalDegree / 2;
}

// Trả về tập hợp tất cả friend ID của user
std::unordered_set<int> Graph::getNeighbors(int id) const {
    auto it = adjList.find(id);
    if (it != adjList.end()) {
        return it->second;
    }
    return {};
}

// Trả về số lượng bạn bè của user
int Graph::getDegree(int id) const {
    auto it = adjList.find(id);
    if (it != adjList.end()) {
        return static_cast<int>(it->second.size());
    }
    return 0;
}

// In toàn bộ adjacency list ra console
void Graph::displayNetwork() const {
    if (users.empty()) {
        std::cout << "  [Mang luoi trong — chua co nguoi dung nao.]\n";
        return;
    }

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "              SOCIAL NETWORK — ADJACENCY LIST               \n";
    std::cout << "============================================================\n";
    std::cout << "  Tong so nguoi dung: " << getUserCount() << "\n";
    std::cout << "  Tong so ket noi   : " << getEdgeCount() << "\n";
    std::cout << "------------------------------------------------------------\n";

    for (const auto& userPair : users) {
        const User& user = userPair.second;
        std::cout << "  " << user.getName() << " (ID: " << user.getId() << ") --> ";

        auto it = adjList.find(user.getId());
        if (it != adjList.end() && !it->second.empty()) {
            bool first = true;
            for (int friendId : it->second) {
                if (!first) std::cout << ", ";
                // Hiển thị tên bạn bè nếu user tồn tại
                auto friendIt = users.find(friendId);
                if (friendIt != users.end()) {
                    std::cout << friendIt->second.getName() << "(" << friendId << ")";
                } else {
                    std::cout << "Unknown(" << friendId << ")";
                }
                first = false;
            }
        } else {
            std::cout << "[Chua co ban be]";
        }
        std::cout << "\n";
    }

    std::cout << "============================================================\n\n";
}
