#ifndef SOCIAL_NETWORK_H
#define SOCIAL_NETWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

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
    // ============ QUẢN LÝ MẠNG ============

    // Thêm người dùng vào mạng
    void addUser(const string& name);

    // Thêm quan hệ bạn bè (cạnh vô hướng)
    void addFriendship(const string& u, const string& v);

    // Xóa người dùng khỏi mạng
    void removeUser(const string& name);

    // Xóa quan hệ bạn bè
    void removeFriendship(const string& u, const string& v);

    // Xóa toàn bộ mạng (dùng khi load từ file)
    void clear();

    // ============ TRUY VẤN ============

    // Lấy danh sách bạn bè của user
    vector<string> getFriends(const string& user) const;

    // Kiểm tra 2 user có phải bạn bè không
    bool areFriends(const string& u, const string& v) const;

    // Kiểm tra user có tồn tại không
    bool userExists(const string& name) const;

    // Số lượng user
    int getUserCount() const;

    // Số lượng cạnh (quan hệ bạn bè)
    int getEdgeCount() const;

    // Lấy danh sách tất cả user
    vector<string> getAllUsers() const;

    // ============ THUẬT TOÁN ============

    // BFS duyệt toàn bộ graph từ start — O(V + E)
    vector<string> bfs(const string& start) const;

    // Đếm số bạn chung giữa 2 user
    int mutualFriendsCount(const string& u, const string& v) const;

    // Gợi ý kết bạn — BFS depth 2, xếp hạng theo số bạn chung
    // Trả về: vector<pair<tên_gợi_ý, số_bạn_chung>> sắp giảm dần
    vector<pair<string, int>> suggestFriends(const string& user) const;

    // ============ HIỂN THỊ ============

    // In toàn bộ adjacency list
    void displayNetwork() const;
};

#endif // SOCIAL_NETWORK_H
