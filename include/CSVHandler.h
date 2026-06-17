#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include <string>
#include "Graph.h"
#include "User.h"

// =============================================================================
// Class CSVHandler
// Xử lý toàn bộ File I/O — đọc dữ liệu từ CSV khi khởi động và ghi thay đổi
// xuống disk sau mỗi thao tác.
//
// THIẾT KẾ — INSTANT SYNC:
//   Mỗi thao tác thêm/xóa đều trigger ghi CSV ngay lập tức, đảm bảo dữ liệu
//   không bị mất ngay cả khi chương trình bị dừng đột ngột.
//   Trade-off: hy sinh một chút hiệu suất để đổi lấy tính an toàn dữ liệu.
//
// Tất cả method đều là STATIC — không cần tạo object, gọi trực tiếp:
//   CSVHandler::loadUsers("data/users.csv", graph);
// =============================================================================
class CSVHandler {
public:
    // =========================================================================
    // LOAD — Đọc dữ liệu từ CSV vào Graph (gọi khi khởi động)
    // =========================================================================

    // -------------------------------------------------------------------------
    // loadUsers()
    // Mở file users.csv, parse từng dòng (id, name, age, location),
    // gọi graph.addUser() cho mỗi bản ghi.
    // Bỏ qua dòng header đầu tiên.
    // -------------------------------------------------------------------------
    static void loadUsers(const std::string& filename, Graph& graph);

    // -------------------------------------------------------------------------
    // loadFriendships()
    // Mở file friendships.csv, parse từng dòng (user_id_1, user_id_2),
    // gọi graph.addEdge() cho mỗi cặp.
    // Bỏ qua dòng header đầu tiên.
    // -------------------------------------------------------------------------
    static void loadFriendships(const std::string& filename, Graph& graph);

    // =========================================================================
    // SAVE — Ghi toàn bộ dữ liệu từ Graph xuống CSV
    // =========================================================================

    // -------------------------------------------------------------------------
    // saveUsers()
    // Ghi tất cả user metadata từ graph xuống file users.csv.
    // Overwrite toàn bộ file với trạng thái hiện tại.
    // Format: id,name,age,location (có header row)
    // -------------------------------------------------------------------------
    static void saveUsers(const std::string& filename, const Graph& graph);

    // -------------------------------------------------------------------------
    // saveFriendships()
    // Ghi tất cả cạnh từ graph xuống file friendships.csv.
    // Mỗi cạnh chỉ ghi 1 lần (quy ước u < v để tránh trùng lặp).
    // Format: user_id_1,user_id_2 (có header row)
    // -------------------------------------------------------------------------
    static void saveFriendships(const std::string& filename, const Graph& graph);

    // =========================================================================
    // APPEND — Thêm 1 bản ghi vào cuối CSV (dùng cho instant sync khi thêm mới)
    // =========================================================================

    // -------------------------------------------------------------------------
    // appendUser()
    // Thêm 1 dòng user vào cuối file users.csv.
    // Dùng khi thêm user mới — nhanh hơn rewrite toàn bộ file.
    // -------------------------------------------------------------------------
    static void appendUser(const std::string& filename, const User& user);

    // -------------------------------------------------------------------------
    // appendFriendship()
    // Thêm 1 dòng cạnh vào cuối file friendships.csv.
    // Dùng khi thêm friendship mới.
    // -------------------------------------------------------------------------
    static void appendFriendship(const std::string& filename, int u, int v);

    // =========================================================================
    // REWRITE — Ghi lại toàn bộ CSV sau khi xóa (vì CSV không hỗ trợ xóa dòng)
    // =========================================================================

    // -------------------------------------------------------------------------
    // rewriteUsers()
    // Ghi lại toàn bộ file users.csv từ dữ liệu hiện tại trong Graph.
    // Gọi sau khi xóa user — cần thiết vì CSV không hỗ trợ random deletion.
    // -------------------------------------------------------------------------
    static void rewriteUsers(const std::string& filename, const Graph& graph);

    // -------------------------------------------------------------------------
    // rewriteFriendships()
    // Ghi lại toàn bộ file friendships.csv từ dữ liệu hiện tại trong Graph.
    // Gọi sau khi xóa edge hoặc xóa user (vì các edge liên quan cũng bị xóa).
    // -------------------------------------------------------------------------
    static void rewriteFriendships(const std::string& filename, const Graph& graph);
};

#endif // CSVHANDLER_H
