#ifndef GRAPH_H
#define GRAPH_H

#include "User.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph {
private:
  // -------------------------------------------------------------------------
  // adjList: Danh sách kề
  //   Key   = user ID
  //   Value = tập hợp các friend ID (unordered_set cho O(1) lookup)
  // Mỗi cạnh (u, v) được lưu ở CẢ HAI phía: adjList[u] chứa v, adjList[v] chứa
  // u
  // -------------------------------------------------------------------------
  std::unordered_map<int, std::unordered_set<int>> adjList;

  // -------------------------------------------------------------------------
  // users: Map lưu metadata người dùng
  //   Key   = user ID
  //   Value = User object (name, age, location)
  // Cho phép truy cập thông tin user theo ID trong O(1) average.
  // -------------------------------------------------------------------------
  std::unordered_map<int, User> users;

public:
  // -------------------------------------------------------------------------
  // addUser()
  // Tạo User mới và thêm vào graph.
  // - Thêm User vào users map
  // - Khởi tạo tập adjacency rỗng trong adjList
  // -------------------------------------------------------------------------
  void addUser(int id, const std::string &name, int age,
               const std::string &location);

  // -------------------------------------------------------------------------
  // removeUser()
  // Xóa user khỏi graph.
  // - Duyệt tất cả neighbor của user, xóa user khỏi adjacency set của mỗi
  // neighbor
  // - Xóa entry của user trong adjList và users map
  // -------------------------------------------------------------------------
  void removeUser(int id);

  // -------------------------------------------------------------------------
  // hasUser()
  // Kiểm tra user có tồn tại trong graph không.
  // -------------------------------------------------------------------------
  bool hasUser(int id) const;

  // -------------------------------------------------------------------------
  // getUser()
  // Trả về User object theo ID.
  // Precondition: user phải tồn tại (kiểm tra bằng hasUser() trước).
  // -------------------------------------------------------------------------
  User getUser(int id) const;

  // -------------------------------------------------------------------------
  // getAllUserIds()
  // Trả về danh sách tất cả user ID trong graph.
  // -------------------------------------------------------------------------
  std::vector<int> getAllUserIds() const;

  // -------------------------------------------------------------------------
  // getUserCount()
  // Trả về tổng số user trong graph.
  // -------------------------------------------------------------------------
  int getUserCount() const;

  // -------------------------------------------------------------------------
  // addEdge()
  // Thêm cạnh vô hướng giữa user u và user v.
  // - Thêm v vào adjList[u] và u vào adjList[v]
  // -------------------------------------------------------------------------
  void addEdge(int u, int v);

  // -------------------------------------------------------------------------
  // removeEdge()
  // Xóa cạnh giữa user u và user v.
  // - Xóa v khỏi adjList[u] và u khỏi adjList[v]
  // -------------------------------------------------------------------------
  void removeEdge(int u, int v);

  // -------------------------------------------------------------------------
  // hasEdge()
  // Kiểm tra hai user có phải bạn bè không.
  // Complexity: O(1) average (hash set lookup)
  // -------------------------------------------------------------------------
  bool hasEdge(int u, int v) const;

  // -------------------------------------------------------------------------
  // getEdgeCount()
  // Trả về tổng số cạnh trong graph.
  // Mỗi cạnh được đếm 1 lần (tổng degree / 2).
  // -------------------------------------------------------------------------
  int getEdgeCount() const;

  // -------------------------------------------------------------------------
  // getNeighbors()
  // Trả về tập hợp tất cả friend ID của user.
  // -------------------------------------------------------------------------
  std::unordered_set<int> getNeighbors(int id) const;

  // -------------------------------------------------------------------------
  // getDegree()
  // Trả về số lượng bạn bè = kích thước adjList[id].
  // -------------------------------------------------------------------------
  int getDegree(int id) const;
  // -------------------------------------------------------------------------
  // displayNetwork()
  // In toàn bộ adjacency list ra console.
  // Format: "User [name] (ID: x) --> friend1, friend2, ..."
  // -------------------------------------------------------------------------
  void displayNetwork() const;
};

#endif // GRAPH_H
