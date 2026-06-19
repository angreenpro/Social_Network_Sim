#include "CSVHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// ── Helper: trim whitespace & carriage return (\r) từ đầu/cuối chuỗi ────────
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// ── Load danh sách users từ file CSV vào Graph ──────────────────────────────
// Format: id,name,age,location  (dòng đầu là header)
void CSVHandler::loadUsers(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[CSVHandler] Khong the mo file: " << filename << "\n";
        return;
    }

    std::string line;
    // Bỏ qua dòng header
    std::getline(file, line);

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;

        // Parse id
        std::getline(ss, token, ',');
        int id = std::stoi(trim(token));

        // Parse name
        std::string name;
        std::getline(ss, name, ',');
        name = trim(name);

        // Parse age
        std::getline(ss, token, ',');
        int age = std::stoi(trim(token));

        // Parse location (phần còn lại của dòng — có thể chứa dấu phẩy)
        std::string location;
        std::getline(ss, location);
        location = trim(location);

        graph.addUser(id, name, age, location);
    }

    file.close();
}

// ── Load danh sách friendships từ file CSV vào Graph ─────────────────────────
// Format: user_id_1,user_id_2  (dòng đầu là header)
void CSVHandler::loadFriendships(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[CSVHandler] Khong the mo file: " << filename << "\n";
        return;
    }

    std::string line;
    // Bỏ qua dòng header
    std::getline(file, line);

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;

        std::getline(ss, token, ',');
        int u = std::stoi(trim(token));

        std::getline(ss, token, ',');
        int v = std::stoi(trim(token));

        // Chỉ thêm edge nếu cả 2 user tồn tại trong graph
        if (graph.hasUser(u) && graph.hasUser(v)) {
            graph.addEdge(u, v);
        }
    }

    file.close();
}

// ── Ghi toàn bộ user metadata từ graph xuống file CSV ────────────────────────
void CSVHandler::saveUsers(const std::string& filename, const Graph& graph) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[CSVHandler] Khong the ghi file: " << filename << "\n";
        return;
    }

    // Ghi header
    file << "id,name,age,location\n";

    // Ghi từng user
    std::vector<int> ids = graph.getAllUserIds();
    // Sắp xếp theo ID tăng dần cho output nhất quán
    std::sort(ids.begin(), ids.end());

    for (int id : ids) {
        User user = graph.getUser(id);
        file << user.getId() << ","
             << user.getName() << ","
             << user.getAge() << ","
             << user.getLocation() << "\n";
    }

    file.close();
}

// ── Ghi toàn bộ mối quan hệ bạn bè từ graph xuống file CSV ──────────────────
void CSVHandler::saveFriendships(const std::string& filename, const Graph& graph) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[CSVHandler] Khong the ghi file: " << filename << "\n";
        return;
    }

    // Ghi header
    file << "user_id_1,user_id_2\n";

    std::vector<int> ids = graph.getAllUserIds();
    std::sort(ids.begin(), ids.end());

    // Ghi mỗi cạnh đúng 1 lần: chỉ ghi khi u < v (đồ thị vô hướng)
    for (int u : ids) {
        std::unordered_set<int> neighbors = graph.getNeighbors(u);
        // Chuyển sang vector để sort cho output nhất quán
        std::vector<int> sortedNeighbors(neighbors.begin(), neighbors.end());
        std::sort(sortedNeighbors.begin(), sortedNeighbors.end());

        for (int v : sortedNeighbors) {
            if (u < v) {
                file << u << "," << v << "\n";
            }
        }
    }

    file.close();
}

// ── Append 1 dòng user vào cuối file CSV (dùng khi thêm mới) ────────────────
void CSVHandler::appendUser(const std::string& filename, const User& user) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[CSVHandler] Khong the mo file de append: " << filename << "\n";
        return;
    }

    file << user.getId() << ","
         << user.getName() << ","
         << user.getAge() << ","
         << user.getLocation() << "\n";

    file.close();
}

// ── Append 1 dòng friendship vào cuối file CSV (dùng khi thêm mới) ──────────
void CSVHandler::appendFriendship(const std::string& filename, int u, int v) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[CSVHandler] Khong the mo file de append: " << filename << "\n";
        return;
    }

    // Luôn ghi u < v cho nhất quán
    if (u > v) std::swap(u, v);
    file << u << "," << v << "\n";

    file.close();
}

// ── Ghi lại toàn bộ file users.csv (thường gọi sau khi xóa user) ────────────
// Về bản chất giống saveUsers — rewrite toàn bộ file
void CSVHandler::rewriteUsers(const std::string& filename, const Graph& graph) {
    saveUsers(filename, graph);
}

// ── Ghi lại toàn bộ file friendships.csv (thường gọi sau khi xóa edge/user) ─
// Về bản chất giống saveFriendships — rewrite toàn bộ file
void CSVHandler::rewriteFriendships(const std::string& filename, const Graph& graph) {
    saveFriendships(filename, graph);
}
