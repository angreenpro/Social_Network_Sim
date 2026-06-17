#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <utility>  // std::pair
#include "Graph.h"

// =============================================================================
// Class Algorithms
// Triển khai các thuật toán đồ thị cốt lõi cho mạng xã hội.
//
// Module này cung cấp "trí tuệ" của ứng dụng:
//   - BFS traversal — duyệt đồ thị theo chiều rộng
//   - Mutual friends — tìm bạn chung giữa 2 user
//   - Friend suggestions — gợi ý kết bạn dựa trên 2-hop BFS
//   - Shortest path — tìm đường đi ngắn nhất (BFS cho đồ thị không trọng số)
//
// Tất cả method đều là STATIC và nhận Graph bằng const& (chỉ đọc, không sửa).
// Menu gọi các hàm này khi user chọn tính năng phân tích.
// =============================================================================
class Algorithms {
public:
    // -------------------------------------------------------------------------
    // bfsTraversal()
    // Duyệt đồ thị theo chiều rộng (Breadth-First Search) từ startId.
    //
    // Thuật toán:
    //   1. Enqueue(start), đánh dấu visited
    //   2. While queue không rỗng:
    //      a. Dequeue node u
    //      b. Với mỗi neighbor v của u:
    //         nếu v chưa visited → enqueue(v), đánh dấu visited
    //   3. Trả về thứ tự duyệt
    //
    // Cấu trúc dữ liệu sử dụng:
    //   - queue<int>          : hàng đợi FIFO cho BFS frontier
    //   - unordered_set<int>  : tập visited — O(1) kiểm tra đã thăm
    //
    // Returns: vector<int> — danh sách user ID theo thứ tự duyệt BFS
    // Complexity: O(V + E)
    // -------------------------------------------------------------------------
    static std::vector<int> bfsTraversal(const Graph& graph, int startId);

    // -------------------------------------------------------------------------
    // getMutualFriends()
    // Tìm bạn chung giữa user u và user v.
    //
    // Thuật toán:
    //   mutual = neighbors(u) ∩ neighbors(v)
    //   Duyệt tập NHỎ HƠN, kiểm tra từng phần tử trong tập LỚN HƠN
    //   (tận dụng O(1) lookup của unordered_set).
    //
    // Returns: vector<int> — danh sách user ID là bạn chung của cả u và v
    // Complexity: O(min(deg(u), deg(v)))
    // -------------------------------------------------------------------------
    static std::vector<int> getMutualFriends(const Graph& graph, int u, int v);

    // -------------------------------------------------------------------------
    // friendSuggestions()
    // TÍNH NĂNG CỐT LÕI — Gợi ý kết bạn dựa trên thuật toán 2-hop BFS.
    //
    // Thuật toán:
    //   1. Lấy S1 = bạn trực tiếp của userId (1-hop neighbors)
    //   2. Với mỗi friend f trong S1, lấy neighbors(f)
    //      → Gom tất cả 2-hop neighbors vào tập ứng viên S2
    //   3. Loại userId và tất cả member của S1 khỏi S2
    //      (không gợi ý chính mình hoặc bạn đã có)
    //   4. Với mỗi ứng viên w trong S2:
    //      Đếm |neighbors(userId) ∩ neighbors(w)| = số bạn chung
    //   5. Sort ứng viên theo số bạn chung GIẢM DẦN
    //   6. Trả về top-K gợi ý dưới dạng pair (userID, mutualCount)
    //
    // Returns: vector<pair<int, int>> — [(userID, số bạn chung), ...] top K
    // Complexity: O(deg² + k·log(k))
    //   — deg² để duyệt 2-hop neighborhood
    //   — k·log(k) để sort k ứng viên
    // -------------------------------------------------------------------------
    static std::vector<std::pair<int, int>> friendSuggestions(const Graph& graph, int userId, int topK);

    // -------------------------------------------------------------------------
    // shortestPath()
    // Tìm đường đi ngắn nhất giữa source và target bằng BFS.
    // BFS đảm bảo tìm shortest path trong đồ thị KHÔNG TRỌNG SỐ
    // (tất cả cạnh có weight = 1).
    //
    // Thuật toán:
    //   - BFS từ source, lưu parent[] map để reconstruct path
    //   - Khi tìm thấy target → truy ngược parent[] để xây dựng đường đi
    //
    // Returns: vector<int> — đường đi từ source đến target (bao gồm cả 2 đầu),
    //          hoặc vector rỗng nếu không có đường đi.
    // Complexity: O(V + E)
    // -------------------------------------------------------------------------
    static std::vector<int> shortestPath(const Graph& graph, int source, int target);
};

#endif // ALGORITHMS_H
