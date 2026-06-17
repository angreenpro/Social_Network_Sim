#include "Algorithms.h"
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// =============================================================================
// bfsTraversal()
// Duyệt đồ thị theo chiều rộng (Breadth-First Search) từ startId.
//
// Thuật toán BFS — sử dụng FIFO queue:
//   1. Enqueue(start), đánh dấu visited
//   2. While queue không rỗng:
//      a. Dequeue node u → thêm vào kết quả
//      b. Với mỗi neighbor v của u:
//         nếu v chưa visited → enqueue(v), đánh dấu visited
//   3. Trả về thứ tự duyệt
//
// TẠI SAO BFS?
//   - BFS duyệt theo "lớp" (level) — tất cả node cách start k bước được
//     thăm trước khi thăm node cách k+1 bước.
//   - Phù hợp cho social network: lớp 1 = bạn trực tiếp,
//     lớp 2 = bạn-của-bạn, v.v.
//
// Cấu trúc dữ liệu:
//   - queue<int>           : FIFO frontier — đảm bảo duyệt theo thứ tự level
//   - unordered_set<int>   : visited set — O(1) kiểm tra đã thăm
//   - vector<int>          : result — lưu thứ tự duyệt
//
// Complexity: O(V + E) — mỗi node và mỗi edge được xét đúng 1 lần
// =============================================================================
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

// =============================================================================
// getMutualFriends()
// Tìm bạn chung giữa user u và user v.
//
// Thuật toán SET INTERSECTION tối ưu:
//   mutual = neighbors(u) ∩ neighbors(v)
//
//   Thay vì dùng std::set_intersection (cần sorted container → O(n log n)),
//   ta tận dụng unordered_set với O(1) lookup:
//     1. Xác định set NHỎ HƠN (smallSet) và set LỚN HƠN (largeSet)
//     2. Iterate qua smallSet, check membership trong largeSet
//     → Tổng: O(min(deg(u), deg(v))) thay vì O(deg(u) + deg(v))
//
// Tối ưu hóa: luôn iterate set nhỏ hơn vì:
//   - Số lần iterate = min(deg(u), deg(v))
//   - Mỗi lần check membership = O(1) average (hash set)
//
// Returns: vector<int> — danh sách user ID là bạn chung của cả u và v
// Complexity: O(min(deg(u), deg(v)))
// =============================================================================
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

// =============================================================================
// friendSuggestions()
// TÍNH NĂNG CỐT LÕI — Gợi ý kết bạn dựa trên thuật toán 2-hop BFS.
//
// Ý tưởng: "Bạn-của-bạn mà chưa phải bạn" — người có nhiều bạn chung nhất
// với userId sẽ được gợi ý trước (giống cơ chế "People You May Know" trên
// Facebook/LinkedIn).
//
// Thuật toán chi tiết:
//   1. Lấy S1 = bạn trực tiếp của userId (1-hop neighbors)
//   2. 2-hop expansion:
//      Với mỗi friend f trong S1 → lấy neighbors(f)
//      → Gom tất cả 2-hop neighbors vào tập ứng viên (candidates)
//   3. Lọc candidates:
//      - Loại userId (không tự gợi ý chính mình)
//      - Loại tất cả member của S1 (đã là bạn rồi, không cần gợi ý)
//   4. Scoring:
//      Với mỗi candidate w, đếm mutual friends = |neighbors(userId) ∩ neighbors(w)|
//      → Dùng unordered_map<int, int> để đếm score cho mỗi candidate
//   5. Sort theo score GIẢM DẦN
//   6. Trả về top-K kết quả
//
// Returns: vector<pair<int, int>> — [(candidateID, mutualCount), ...] top K
// Complexity: O(deg² + k·log(k))
//   — deg² để duyệt toàn bộ 2-hop neighborhood
//   — k·log(k) để sort k candidates
// =============================================================================
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

// =============================================================================
// shortestPath()
// Tìm đường đi ngắn nhất giữa source và target bằng BFS.
//
// TẠI SAO BFS CHỨ KHÔNG PHẢI DIJKSTRA?
//   Đồ thị social network là UNWEIGHTED (tất cả edges có weight = 1).
//   Với unweighted graph, BFS đảm bảo tìm shortest path vì:
//   - BFS duyệt theo level → node được discover sớm nhất = đường đi ngắn nhất
//   - Dijkstra cũng đúng nhưng overhead priority queue không cần thiết
//
// Thuật toán:
//   1. BFS từ source
//   2. Lưu parent[v] = u cho mỗi edge u→v được discover
//   3. Khi tìm thấy target → truy ngược parent[] để reconstruct path
//   4. Reverse path (vì reconstruct từ target → source)
//
// Edge case:
//   - source == target → trả về [source]
//   - Không có đường đi → trả về vector rỗng
//
// Returns: vector<int> — đường đi từ source đến target (inclusive cả 2 đầu)
// Complexity: O(V + E)
// =============================================================================
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
