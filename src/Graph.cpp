#include "Graph.h"
#include <iostream>

// =============================================================================
// addUser()
// Tạo User mới và thêm vào graph.
//   1. Tạo User object với thông tin được cung cấp
//   2. Insert vào users map (key = id)
//   3. Khởi tạo empty adjacency set trong adjList (đảm bảo node tồn tại
//      trong đồ thị ngay cả khi chưa có edge nào)
//
// Complexity: O(1) average — hash map insert
// =============================================================================
void Graph::addUser(int id, const std::string& name, int age, const std::string& location) {
    users[id] = User(id, name, age, location);
    adjList[id]; // Khởi tạo empty set nếu chưa tồn tại (operator[] auto-creates)
}

// =============================================================================
// removeUser()
// Xóa user khỏi graph, đảm bảo tính nhất quán (consistency):
//   1. Duyệt tất cả neighbor của user
//   2. Xóa user khỏi adjacency set của MỖI neighbor (vì đồ thị vô hướng,
//      nếu u→v tồn tại thì v→u cũng tồn tại → phải xóa cả hai)
//   3. Xóa entry của user trong adjList
//   4. Xóa entry của user trong users map
//
// Complexity: O(degree(id)) — phải cập nhật tất cả neighbor
// =============================================================================
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

// =============================================================================
// hasUser()
// Kiểm tra user có tồn tại trong graph không.
// Dùng users map (không phải adjList) vì user có thể tồn tại mà chưa có edge.
//
// Complexity: O(1) average — hash map lookup (find)
// =============================================================================
bool Graph::hasUser(int id) const {
    return users.find(id) != users.end();
}

// =============================================================================
// getUser()
// Trả về User object theo ID.
// Precondition: user phải tồn tại — gọi hasUser() kiểm tra trước.
// Dùng .at() thay vì [] để throw exception nếu key không tồn tại
// (an toàn hơn trong const context).
// =============================================================================
User Graph::getUser(int id) const {
    return users.at(id);
}

// =============================================================================
// getAllUserIds()
// Trả về vector chứa tất cả user ID trong graph.
// Iterate qua users map, collect key (first) vào vector.
//
// Complexity: O(V) — phải duyệt toàn bộ map
// =============================================================================
std::vector<int> Graph::getAllUserIds() const {
    std::vector<int> ids;
    ids.reserve(users.size()); // Pre-allocate để tránh reallocations
    for (const auto& pair : users) {
        ids.push_back(pair.first);
    }
    return ids;
}

// =============================================================================
// getUserCount()
// Trả về tổng số user trong graph = kích thước users map.
//
// Complexity: O(1)
// =============================================================================
int Graph::getUserCount() const {
    return static_cast<int>(users.size());
}

// =============================================================================
// addEdge()
// Thêm cạnh vô hướng giữa user u và user v.
// Vì đồ thị UNDIRECTED → cần thêm ở CẢ HAI phía:
//   - Thêm v vào adjList[u]
//   - Thêm u vào adjList[v]
//
// unordered_set::insert() tự động bỏ qua nếu phần tử đã tồn tại (no-op),
// nên gọi addEdge() trùng lặp cũng an toàn (idempotent).
//
// Complexity: O(1) average — hash set insert
// =============================================================================
void Graph::addEdge(int u, int v) {
    adjList[u].insert(v);
    adjList[v].insert(u);
}

// =============================================================================
// removeEdge()
// Xóa cạnh giữa user u và user v (cả hai chiều).
//
// Complexity: O(1) average — hash set erase
// =============================================================================
void Graph::removeEdge(int u, int v) {
    adjList[u].erase(v);
    adjList[v].erase(u);
}

// =============================================================================
// hasEdge()
// Kiểm tra hai user có phải bạn bè không.
// Chỉ cần kiểm tra 1 chiều (vì addEdge luôn thêm cả 2 chiều).
// Dùng find() trên const reference để tránh tạo entry mới trong map.
//
// Complexity: O(1) average — hash set lookup
// =============================================================================
bool Graph::hasEdge(int u, int v) const {
    auto it = adjList.find(u);
    if (it == adjList.end()) return false;
    return it->second.count(v) > 0;
}

// =============================================================================
// getEdgeCount()
// Trả về tổng số cạnh trong graph.
// Vì mỗi cạnh được lưu 2 lần (u→v và v→u), tổng degree / 2 = số cạnh thực.
//
// Complexity: O(V) — phải duyệt qua tất cả adjacency sets
// =============================================================================
int Graph::getEdgeCount() const {
    int totalDegree = 0;
    for (const auto& pair : adjList) {
        totalDegree += static_cast<int>(pair.second.size());
    }
    return totalDegree / 2;
}

// =============================================================================
// getNeighbors()
// Trả về tập hợp (unordered_set) tất cả friend ID của user.
// Nếu user không tồn tại trong adjList → trả về empty set.
//
// Complexity: O(1) lookup, O(degree) để copy set
// =============================================================================
std::unordered_set<int> Graph::getNeighbors(int id) const {
    auto it = adjList.find(id);
    if (it != adjList.end()) {
        return it->second;
    }
    return {};
}

// =============================================================================
// getDegree()
// Trả về số lượng bạn bè = kích thước adjacency set của user.
// Nếu user không tồn tại → return 0.
//
// Complexity: O(1) — unordered_set::size() là O(1)
// =============================================================================
int Graph::getDegree(int id) const {
    auto it = adjList.find(id);
    if (it != adjList.end()) {
        return static_cast<int>(it->second.size());
    }
    return 0;
}

// =============================================================================
// displayNetwork()
// In toàn bộ adjacency list ra console theo format dễ đọc:
//   User [Name] (ID: x) --> friend1_name(id), friend2_name(id), ...
//
// Nếu graph rỗng → in thông báo tương ứng.
// =============================================================================
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
