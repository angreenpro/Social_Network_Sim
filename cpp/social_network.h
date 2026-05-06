#ifndef SOCIAL_NETWORK_H
#define SOCIAL_NETWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <map>

using namespace std;

/**
 * SocialNetwork — Mô phỏng mạng xã hội bằng đồ thị vô hướng
 * 
 * Cấu trúc dữ liệu: Adjacency List
 *   - unordered_map<string, unordered_set<string>>
 *   - Lý do: Mạng xã hội là sparse graph, adjacency list tiết kiệm O(V+E)
 *     so với adjacency matrix O(V²)
 *   - unordered_set cho phép kiểm tra bạn bè O(1) trung bình
 */
class SocialNetwork {
private:
    // Adjacency List — đồ thị vô hướng
    unordered_map<string, unordered_set<string>> adjList;

public:

    // =============================================
    //  QUẢN LÝ MẠNG
    // =============================================

    // Thêm người dùng vào mạng — O(1)
    void addUser(const string& name) {
        // Nếu user chưa tồn tại, tạo entry mới với tập bạn bè rỗng
        if (adjList.find(name) == adjList.end()) {
            adjList[name] = unordered_set<string>();
        }
    }

    // Thêm quan hệ bạn bè (cạnh vô hướng) — O(1)
    void addFriendship(const string& u, const string& v) {
        // Đảm bảo cả 2 user tồn tại
        addUser(u);
        addUser(v);
        // Thêm cạnh 2 chiều (đồ thị vô hướng)
        adjList[u].insert(v);
        adjList[v].insert(u);
    }

    // Xóa người dùng khỏi mạng — O(degree(name))
    void removeUser(const string& name) {
        if (adjList.find(name) == adjList.end()) return;

        // Xóa name khỏi danh sách bạn bè của tất cả bạn
        for (const string& friendName : adjList[name]) {
            adjList[friendName].erase(name);
        }
        // Xóa entry của name
        adjList.erase(name);
    }

    // Xóa quan hệ bạn bè — O(1)
    void removeFriendship(const string& u, const string& v) {
        if (adjList.find(u) != adjList.end()) adjList[u].erase(v);
        if (adjList.find(v) != adjList.end()) adjList[v].erase(u);
    }

    // Xóa toàn bộ mạng — O(V + E)
    void clear() {
        adjList.clear();
    }
 
    //  TRUY VẤN

    // Lấy danh sách bạn bè (sắp xếp alphabet) — O(d log d), d = degree
    vector<string> getFriends(const string& user) const {
        vector<string> friends;
        auto it = adjList.find(user);
        if (it != adjList.end()) {
            friends.assign(it->second.begin(), it->second.end());
            sort(friends.begin(), friends.end());
        }
        return friends;
    }

    // Kiểm tra 2 user có phải bạn bè — O(1) trung bình
    bool areFriends(const string& u, const string& v) const {
        auto it = adjList.find(u);
        if (it == adjList.end()) return false;
        return it->second.count(v) > 0;
    }

    // Kiểm tra user có tồn tại — O(1) trung bình
    bool userExists(const string& name) const {
        return adjList.find(name) != adjList.end();
    }

    // Số lượng user — O(1)
    int getUserCount() const {
        return adjList.size();
    }

    // Số lượng cạnh — O(V)
    // Mỗi cạnh được lưu 2 lần (vô hướng), nên tổng / 2
    int getEdgeCount() const {
        int total = 0;
        for (const auto& pair : adjList) {
            total += pair.second.size();
        }
        return total / 2;
    }

    // Lấy danh sách tất cả user (sắp xếp) — O(V log V)
    vector<string> getAllUsers() const {
        vector<string> users;
        for (const auto& pair : adjList) {
            users.push_back(pair.first);
        }
        sort(users.begin(), users.end());
        return users;
    }

    //  THUẬT TOÁN

    /**
     * BFS — Breadth-First Search từ node start
     * 
     * Cách hoạt động:
     *   1. Đưa start vào queue, đánh dấu visited
     *   2. Lặp: lấy node đầu queue ra
     *      - Thêm vào kết quả
     *      - Duyệt tất cả neighbor chưa visited
     *      - Đưa neighbor vào queue, đánh dấu visited
     *   3. Lặp cho đến khi queue rỗng
     * 
     * Độ phức tạp: O(V + E) thời gian, O(V) không gian
     */
    vector<string> bfs(const string& start) const {
        vector<string> result;

        if (adjList.find(start) == adjList.end()) return result;

        unordered_set<string> visited;
        queue<string> q;

        // Bước 1: Khởi tạo — đưa start vào queue
        q.push(start);
        visited.insert(start);

        // Bước 2: Lặp cho đến khi queue rỗng
        while (!q.empty()) {
            // Lấy node đầu queue
            string current = q.front();
            q.pop();

            // Thêm vào kết quả duyệt
            result.push_back(current);

            // Duyệt tất cả neighbor
            vector<string> neighbors = getFriends(current);
            for (const string& neighbor : neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    // Neighbor chưa visited → đánh dấu và đưa vào queue
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }

        return result;
    }

    /**
     * Đếm số bạn chung giữa 2 user
     * 
     * Cách: lấy giao của 2 tập bạn bè
     * Độ phức tạp: O(min(d1, d2)) trung bình, d = degree
     */
    int mutualFriendsCount(const string& u, const string& v) const {
        auto itU = adjList.find(u);
        auto itV = adjList.find(v);
        if (itU == adjList.end() || itV == adjList.end()) return 0;

        int count = 0;
        // Duyệt tập nhỏ hơn để tối ưu
        const unordered_set<string>& smaller = (itU->second.size() <= itV->second.size()) 
                                                ? itU->second : itV->second;
        const unordered_set<string>& larger = (itU->second.size() <= itV->second.size()) 
                                               ? itV->second : itU->second;
        
        for (const string& f : smaller) {
            if (larger.count(f) > 0) {
                count++;
            }
        }
        return count;
    }

    /**
     * Gợi ý kết bạn — "Người quen của người quen"
     * 
     * Thuật toán:
     *   1. Duyệt tất cả bạn bè (depth 1) của user
     *   2. Với mỗi bạn, duyệt bạn của bạn đó (depth 2)
     *   3. Nếu bạn-của-bạn ≠ user VÀ chưa là bạn trực tiếp
     *      → đó là ứng viên gợi ý, +1 bạn chung
     *   4. Sắp xếp theo số bạn chung giảm dần
     * 
     * Độ phức tạp: O(d1 × d2) trong đó d1 = degree(user), d2 = max degree bạn
     */
    vector<pair<string, int>> suggestFriends(const string& user) const {
        auto it = adjList.find(user);
        if (it == adjList.end()) return {};

        const unordered_set<string>& friends = it->second;

        // Map: ứng viên → số bạn chung
        map<string, int> candidates;

        // Duyệt bạn bè depth 1
        for (const string& friendName : friends) {
            auto friendIt = adjList.find(friendName);
            if (friendIt == adjList.end()) continue;

            // Duyệt bạn của bạn (depth 2)
            for (const string& fof : friendIt->second) {
                // Bỏ qua chính user và những người đã là bạn
                if (fof == user) continue;
                if (friends.count(fof) > 0) continue;

                // fof là ứng viên, friendName là bạn chung
                candidates[fof]++;
            }
        }

        // Chuyển sang vector và sắp xếp theo số bạn chung giảm dần
        vector<pair<string, int>> result(candidates.begin(), candidates.end());
        sort(result.begin(), result.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            if (a.second != b.second) return a.second > b.second;  // nhiều bạn chung trước
            return a.first < b.first;  // cùng số thì sắp alphabet
        });

        return result;
    }

    //  HIỂN THỊ

    // In toàn bộ adjacency list — O(V + E)
    void displayNetwork() const {
        cout << "--- Mang xa hoi (Adjacency List) ---" << endl;

        vector<string> users = getAllUsers();
        for (const string& user : users) {
            cout << "  " << user << " -> ";
            vector<string> friends = getFriends(user);
            for (int i = 0; i < friends.size(); i++) {
                if (i > 0) cout << ", ";
                cout << friends[i];
            }
            cout << endl;
        }

        cout << "------------------------------------" << endl;
        cout << "Tong: " << getUserCount() << " users, " 
             << getEdgeCount() << " connections" << endl;
    }
};

#endif // SOCIAL_NETWORK_H
