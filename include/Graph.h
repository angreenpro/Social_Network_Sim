#ifndef GRAPH_H
#define GRAPH_H

#include "User.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph {
private:
  // adjList: Danh sách kề lưu mối quan hệ bạn bè
  std::unordered_map<int, std::unordered_set<int>> adjList;

  // users: Lưu thông tin chi tiết người dùng
  std::unordered_map<int, User> users;

public:
  // Tạo User mới và thêm vào graph
  void addUser(int id, const std::string &name, int age,
               const std::string &location);

  // Xóa user khỏi graph
  void removeUser(int id);

  // Kiểm tra user có tồn tại trong graph không
  bool hasUser(int id) const;

  // Trả về User object theo ID
  User getUser(int id) const;

  // Trả về danh sách tất cả user ID
  std::vector<int> getAllUserIds() const;

  // Trả về tổng số user trong graph
  int getUserCount() const;

  // Thêm mối quan hệ bạn bè (cạnh vô hướng)
  void addEdge(int u, int v);

  // Xóa mối quan hệ bạn bè
  void removeEdge(int u, int v);

  // Kiểm tra hai user có phải bạn bè không
  bool hasEdge(int u, int v) const;

  // Trả về tổng số lượng mối quan hệ
  int getEdgeCount() const;

  // Trả về danh sách bạn bè của user
  std::unordered_set<int> getNeighbors(int id) const;

  // Trả về số lượng bạn bè của user
  int getDegree(int id) const;

  // In mạng lưới ra console
  void displayNetwork() const;
};

#endif // GRAPH_H
