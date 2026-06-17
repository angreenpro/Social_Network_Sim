#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <utility>  // std::pair
#include "Graph.h"

// Lớp cung cấp các thuật toán cốt lõi cho mạng xã hội
class Algorithms {
public:
    // Duyệt đồ thị theo chiều rộng (BFS) từ startId
    static std::vector<int> bfsTraversal(const Graph& graph, int startId);

    // Tìm danh sách bạn bè chung giữa hai người dùng
    static std::vector<int> getMutualFriends(const Graph& graph, int u, int v);

    // Gợi ý kết bạn dựa trên số lượng bạn chung (sử dụng 2-hop BFS)
    static std::vector<std::pair<int, int>> friendSuggestions(const Graph& graph, int userId, int topK);

    // Tìm đường đi ngắn nhất giữa hai người dùng (sử dụng BFS)
    static std::vector<int> shortestPath(const Graph& graph, int source, int target);
};

#endif // ALGORITHMS_H
