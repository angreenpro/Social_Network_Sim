#include "Algorithms.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// Duyệt đồ thị theo chiều rộng (BFS) từ startId
std::vector<int> Algorithms::bfsTraversal(const Graph& graph, int startId) {
    std::vector<int> result;

    // Kiểm tra startId có tồn tại trong graph không
    if (!graph.hasUser(startId)) {
        return result;
    }

    std::queue<int> frontier;           // FIFO queue cho BFS
    std::unordered_set<int> visited;    // Tập các node đã thăm

    // Bước 1: Khởi tạo — enqueue start node
    frontier.push(startId);
    visited.insert(startId);

    // Bước 2: Lặp cho đến khi frontier rỗng
    while (!frontier.empty()) {
        // Dequeue node đầu tiên
        int current = frontier.front();
        frontier.pop();

        // Thêm vào kết quả (theo thứ tự dequeue = thứ tự BFS)
        result.push_back(current);

        // Duyệt tất cả neighbor của current
        std::unordered_set<int> neighbors = graph.getNeighbors(current);
        for (int neighbor : neighbors) {
            // Nếu chưa thăm → enqueue và đánh dấu visited
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                frontier.push(neighbor);
            }
        }
    }

    return result;
}

// Tìm danh sách bạn bè chung giữa hai người dùng (sử dụng phép giao tập hợp)
std::vector<int> Algorithms::getMutualFriends(const Graph& graph, int u, int v) {
    std::vector<int> mutual;

    // Kiểm tra cả 2 user tồn tại
    if (!graph.hasUser(u) || !graph.hasUser(v)) {
        return mutual;
    }

    std::unordered_set<int> neighborsU = graph.getNeighbors(u);
    std::unordered_set<int> neighborsV = graph.getNeighbors(v);

    // Chọn set nhỏ hơn để iterate (tối ưu hiệu suất)
    const std::unordered_set<int>& smallSet = (neighborsU.size() <= neighborsV.size()) ? neighborsU : neighborsV;
    const std::unordered_set<int>& largeSet = (neighborsU.size() <= neighborsV.size()) ? neighborsV : neighborsU;

    // Iterate set nhỏ, check membership trong set lớn
    for (int friendId : smallSet) {
        if (largeSet.count(friendId) > 0) {
            mutual.push_back(friendId);
        }
    }

    return mutual;
}

// Gợi ý kết bạn dựa trên số lượng bạn chung (sử dụng thuật toán 2-hop BFS)
std::vector<std::pair<int, int>> Algorithms::friendSuggestions(const Graph& graph, int userId, int topK) {
    std::vector<std::pair<int, int>> suggestions;

    if (!graph.hasUser(userId)) {
        return suggestions;
    }

    // Bước 1: Lấy direct friends (1-hop neighbors)
    std::unordered_set<int> directFriends = graph.getNeighbors(userId);

    // Bước 2 + 3: Thu thập candidates (2-hop neighbors, loại userId và direct friends)
    // Dùng unordered_map để đếm số lần xuất hiện = số bạn chung
    // (mỗi lần candidate w xuất hiện qua friend f khác nhau → thêm 1 mutual friend)
    std::unordered_map<int, int> candidateScore;

    for (int friendId : directFriends) {
        std::unordered_set<int> friendsOfFriend = graph.getNeighbors(friendId);
        for (int candidate : friendsOfFriend) {
            // Loại chính userId và các direct friend đã có
            if (candidate != userId && directFriends.count(candidate) == 0) {
                candidateScore[candidate]++;
            }
        }
    }

    // Bước 4: Chuyển map sang vector để sort
    for (const auto& pair : candidateScore) {
        suggestions.push_back({pair.first, pair.second});
    }

    // Bước 5: Sort GIẢM DẦN theo mutual count
    // Nếu bằng nhau → sort tăng dần theo ID (deterministic output)
    std::sort(suggestions.begin(), suggestions.end(),
        [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.second != b.second) return a.second > b.second;  // Giảm dần theo score
            return a.first < b.first;                               // Tăng dần theo ID
        });

    // Bước 6: Giới hạn kết quả ở top-K
    if (topK > 0 && static_cast<int>(suggestions.size()) > topK) {
        suggestions.resize(topK);
    }

    return suggestions;
}

// Tìm đường đi ngắn nhất giữa source và target bằng BFS
std::vector<int> Algorithms::shortestPath(const Graph& graph, int source, int target) {
    std::vector<int> path;

    // Kiểm tra cả 2 node tồn tại
    if (!graph.hasUser(source) || !graph.hasUser(target)) {
        return path;
    }

    // Edge case: source chính là target
    if (source == target) {
        path.push_back(source);
        return path;
    }

    // BFS với parent tracking
    std::queue<int> frontier;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parent;  // parent[v] = u → "v được discover từ u"

    frontier.push(source);
    visited.insert(source);

    bool found = false;

    while (!frontier.empty() && !found) {
        int current = frontier.front();
        frontier.pop();

        std::unordered_set<int> neighbors = graph.getNeighbors(current);
        for (int neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                frontier.push(neighbor);

                // Tìm thấy target → dừng BFS sớm (shortest path đã được tìm)
                if (neighbor == target) {
                    found = true;
                    break;
                }
            }
        }
    }

    // Nếu không tìm thấy đường đi → trả về rỗng
    if (!found) {
        return path;
    }

    // Reconstruct path: truy ngược từ target → source qua parent map
    int current = target;
    while (current != source) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(source);

    // Reverse vì ta build từ target → source, cần source → target
    std::reverse(path.begin(), path.end());

    return path;
}
