#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include <string>
#include "Graph.h"
#include "User.h"

// Lớp CSVHandler xử lý File I/O: đọc dữ liệu từ CSV và ghi thay đổi xuống file
class CSVHandler {
public:
    // Đọc dữ liệu users từ file CSV vào Graph
    static void loadUsers(const std::string& filename, Graph& graph);

    // Đọc dữ liệu friendships từ file CSV vào Graph
    static void loadFriendships(const std::string& filename, Graph& graph);

    // Ghi toàn bộ user metadata từ graph xuống file CSV
    static void saveUsers(const std::string& filename, const Graph& graph);

    // Ghi toàn bộ mối quan hệ bạn bè từ graph xuống file CSV
    static void saveFriendships(const std::string& filename, const Graph& graph);

    // Thêm 1 dòng user vào cuối file CSV (dùng khi thêm mới)
    static void appendUser(const std::string& filename, const User& user);

    // Thêm 1 dòng friendship vào cuối file CSV (dùng khi thêm mới)
    static void appendFriendship(const std::string& filename, int u, int v);

    // Ghi lại toàn bộ file users.csv (thường gọi sau khi xóa user)
    static void rewriteUsers(const std::string& filename, const Graph& graph);

    // Ghi lại toàn bộ file friendships.csv (thường gọi sau khi xóa edge hoặc user)
    static void rewriteFriendships(const std::string& filename, const Graph& graph);
};

#endif // CSVHANDLER_H
