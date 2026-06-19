#include "Menu.h"
#include "Algorithms.h"
#include "CSVHandler.h"
#include "Visualizer.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>

// ── Đường dẫn file CSV (relative — sẽ tìm trong thư mục chạy) ──────────────
static const std::string USERS_FILE       = "data/users.csv";
static const std::string FRIENDSHIPS_FILE = "data/friendships.csv";

// ── Helper: đọc int an toàn từ stdin, trả false nếu input không hợp lệ ──────
static bool readInt(const std::string& prompt, int& out) {
    std::cout << prompt;
    if (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [Loi] Vui long nhap so nguyen.\n";
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

// ── Helper: đọc string (cho phép khoảng trắng) ──────────────────────────────
static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// ══════════════════════════════════════════════════════════════════════════════
// Hiển thị menu chính
// ══════════════════════════════════════════════════════════════════════════════
void Menu::displayMenu() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "           SOCIAL NETWORK SIMULATOR — MAIN MENU             \n";
    std::cout << "============================================================\n";
    std::cout << "  [1]  Them nguoi dung\n";
    std::cout << "  [2]  Xoa nguoi dung\n";
    std::cout << "  [3]  Them moi quan he ban be\n";
    std::cout << "  [4]  Xoa moi quan he ban be\n";
    std::cout << "  [5]  Xem so luong ban be\n";
    std::cout << "  [6]  Xem danh sach ban chung\n";
    std::cout << "  [7]  BFS Traversal tu mot user\n";
    std::cout << "  [8]  Tim duong di ngan nhat\n";
    std::cout << "  [9]  Hien thi mang luoi ket noi\n";
    std::cout << "  [10] Goi y ket ban\n";
    std::cout << "  [11] Truc quan hoa do thi\n";
    std::cout << "  [0]  Thoat\n";
    std::cout << "============================================================\n";
}

// ══════════════════════════════════════════════════════════════════════════════
// Vòng lặp chính — dispatch lựa chọn đến handler tương ứng
// ══════════════════════════════════════════════════════════════════════════════
void Menu::run(Graph& graph) {
    int choice = -1;

    while (true) {
        displayMenu();

        if (!readInt("  Lua chon cua ban: ", choice)) {
            continue;
        }

        std::cout << "\n";

        switch (choice) {
            case 1:  handleAddUser(graph);           break;
            case 2:  handleRemoveUser(graph);        break;
            case 3:  handleAddFriendship(graph);     break;
            case 4:  handleRemoveFriendship(graph);  break;
            case 5:  handleFriendCount(graph);       break;
            case 6:  handleMutualFriends(graph);     break;
            case 7:  handleBFS(graph);               break;
            case 8:  handleShortestPath(graph);      break;
            case 9:  handleDisplayNetwork(graph);    break;
            case 10: handleFriendSuggestions(graph); break;
            case 11: handleVisualize(graph);         break;
            case 0:
                std::cout << "  Tam biet! Hen gap lai.\n\n";
                return;
            default:
                std::cout << "  [Loi] Lua chon khong hop le. Vui long chon 0-11.\n";
                break;
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// [1] Thêm người dùng
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleAddUser(Graph& graph) {
    std::cout << "--- THEM NGUOI DUNG ---\n";

    int id;
    if (!readInt("  Nhap ID: ", id)) return;

    if (graph.hasUser(id)) {
        std::cout << "  [Loi] User ID " << id << " da ton tai!\n";
        return;
    }

    std::string name = readLine("  Nhap ten: ");
    if (name.empty()) {
        std::cout << "  [Loi] Ten khong duoc de trong.\n";
        return;
    }

    int age;
    if (!readInt("  Nhap tuoi: ", age)) return;
    if (age <= 0 || age > 150) {
        std::cout << "  [Loi] Tuoi khong hop le.\n";
        return;
    }

    std::string location = readLine("  Nhap noi o: ");
    if (location.empty()) {
        std::cout << "  [Loi] Noi o khong duoc de trong.\n";
        return;
    }

    // Thêm vào graph
    graph.addUser(id, name, age, location);

    // Đồng bộ xuống CSV ngay lập tức
    CSVHandler::appendUser(USERS_FILE, graph.getUser(id));

    std::cout << "  [OK] Da them: " << graph.getUser(id).toString() << "\n";
}

// ══════════════════════════════════════════════════════════════════════════════
// [2] Xóa người dùng
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleRemoveUser(Graph& graph) {
    std::cout << "--- XOA NGUOI DUNG ---\n";

    int id;
    if (!readInt("  Nhap ID can xoa: ", id)) return;

    if (!graph.hasUser(id)) {
        std::cout << "  [Loi] User ID " << id << " khong ton tai.\n";
        return;
    }

    User user = graph.getUser(id);
    std::cout << "  Ban co chac muon xoa " << user.getName() << " (ID: " << id << ")? (y/n): ";
    std::string confirm;
    std::getline(std::cin, confirm);
    if (confirm != "y" && confirm != "Y") {
        std::cout << "  Da huy thao tac xoa.\n";
        return;
    }

    // Xóa khỏi graph (tự động xóa tất cả edge liên quan)
    graph.removeUser(id);

    // Rewrite cả 2 file CSV (vì xóa user ảnh hưởng cả friendships)
    CSVHandler::rewriteUsers(USERS_FILE, graph);
    CSVHandler::rewriteFriendships(FRIENDSHIPS_FILE, graph);

    std::cout << "  [OK] Da xoa user " << user.getName() << " (ID: " << id << ")\n";
}

// ══════════════════════════════════════════════════════════════════════════════
// [3] Thêm mối quan hệ bạn bè
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleAddFriendship(Graph& graph) {
    std::cout << "--- THEM MOI QUAN HE BAN BE ---\n";

    int u, v;
    if (!readInt("  Nhap ID nguoi 1: ", u)) return;
    if (!readInt("  Nhap ID nguoi 2: ", v)) return;

    if (u == v) {
        std::cout << "  [Loi] Khong the ket ban voi chinh minh.\n";
        return;
    }

    if (!graph.hasUser(u)) {
        std::cout << "  [Loi] User ID " << u << " khong ton tai.\n";
        return;
    }
    if (!graph.hasUser(v)) {
        std::cout << "  [Loi] User ID " << v << " khong ton tai.\n";
        return;
    }

    if (graph.hasEdge(u, v)) {
        std::cout << "  [Loi] " << graph.getUser(u).getName() << " va "
                  << graph.getUser(v).getName() << " da la ban be roi.\n";
        return;
    }

    // Thêm edge vào graph
    graph.addEdge(u, v);

    // Đồng bộ xuống CSV
    CSVHandler::appendFriendship(FRIENDSHIPS_FILE, u, v);

    std::cout << "  [OK] Da ket ban: "
              << graph.getUser(u).getName() << " <-> "
              << graph.getUser(v).getName() << "\n";
}

// ══════════════════════════════════════════════════════════════════════════════
// [4] Xóa mối quan hệ bạn bè
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleRemoveFriendship(Graph& graph) {
    std::cout << "--- XOA MOI QUAN HE BAN BE ---\n";

    int u, v;
    if (!readInt("  Nhap ID nguoi 1: ", u)) return;
    if (!readInt("  Nhap ID nguoi 2: ", v)) return;

    if (!graph.hasUser(u) || !graph.hasUser(v)) {
        std::cout << "  [Loi] Mot trong hai user khong ton tai.\n";
        return;
    }

    if (!graph.hasEdge(u, v)) {
        std::cout << "  [Loi] Hai nguoi nay chua phai ban be.\n";
        return;
    }

    // Xóa edge khỏi graph
    graph.removeEdge(u, v);

    // Rewrite file friendships
    CSVHandler::rewriteFriendships(FRIENDSHIPS_FILE, graph);

    std::cout << "  [OK] Da huy ket ban: "
              << graph.getUser(u).getName() << " <-> "
              << graph.getUser(v).getName() << "\n";
}

// ══════════════════════════════════════════════════════════════════════════════
// [5] Xem số lượng bạn bè
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleFriendCount(const Graph& graph) {
    std::cout << "--- SO LUONG BAN BE ---\n";

    int id;
    if (!readInt("  Nhap User ID: ", id)) return;

    if (!graph.hasUser(id)) {
        std::cout << "  [Loi] User ID " << id << " khong ton tai.\n";
        return;
    }

    User user = graph.getUser(id);
    int degree = graph.getDegree(id);

    std::cout << "  " << user.getName() << " (ID: " << id << ") co "
              << degree << " ban be.\n";

    // Liệt kê danh sách bạn bè
    if (degree > 0) {
        std::cout << "  Danh sach: ";
        std::unordered_set<int> neighbors = graph.getNeighbors(id);
        std::vector<int> sorted(neighbors.begin(), neighbors.end());
        std::sort(sorted.begin(), sorted.end());

        bool first = true;
        for (int fid : sorted) {
            if (!first) std::cout << ", ";
            if (graph.hasUser(fid)) {
                std::cout << graph.getUser(fid).getName() << "(" << fid << ")";
            }
            first = false;
        }
        std::cout << "\n";
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// [6] Xem danh sách bạn chung
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleMutualFriends(const Graph& graph) {
    std::cout << "--- BAN CHUNG ---\n";

    int u, v;
    if (!readInt("  Nhap ID nguoi 1: ", u)) return;
    if (!readInt("  Nhap ID nguoi 2: ", v)) return;

    if (!graph.hasUser(u)) {
        std::cout << "  [Loi] User ID " << u << " khong ton tai.\n";
        return;
    }
    if (!graph.hasUser(v)) {
        std::cout << "  [Loi] User ID " << v << " khong ton tai.\n";
        return;
    }

    std::vector<int> mutual = Algorithms::getMutualFriends(graph, u, v);

    std::cout << "  " << graph.getUser(u).getName() << " va "
              << graph.getUser(v).getName() << " co "
              << mutual.size() << " ban chung";

    if (mutual.empty()) {
        std::cout << ".\n";
    } else {
        std::cout << ":\n";
        std::sort(mutual.begin(), mutual.end());
        for (int mid : mutual) {
            if (graph.hasUser(mid)) {
                User m = graph.getUser(mid);
                std::cout << "    - " << m.toString() << "\n";
            }
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// [7] BFS Traversal
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleBFS(const Graph& graph) {
    std::cout << "--- BFS TRAVERSAL ---\n";

    int startId;
    if (!readInt("  Nhap ID bat dau: ", startId)) return;

    if (!graph.hasUser(startId)) {
        std::cout << "  [Loi] User ID " << startId << " khong ton tai.\n";
        return;
    }

    std::vector<int> order = Algorithms::bfsTraversal(graph, startId);

    std::cout << "  Thu tu BFS tu " << graph.getUser(startId).getName()
              << " (ID: " << startId << "):\n";
    std::cout << "  Tong so node duyet duoc: " << order.size()
              << " / " << graph.getUserCount() << "\n\n";

    // Hiển thị theo từng level (ước lượng)
    int count = 0;
    for (int id : order) {
        if (graph.hasUser(id)) {
            User u = graph.getUser(id);
            std::cout << "    [" << (count + 1) << "] "
                      << u.getName() << " (ID: " << id << ")\n";
        }
        count++;
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// [8] Tìm đường đi ngắn nhất
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleShortestPath(const Graph& graph) {
    std::cout << "--- DUONG DI NGAN NHAT (BFS) ---\n";

    int source, target;
    if (!readInt("  Nhap ID nguon: ", source)) return;
    if (!readInt("  Nhap ID dich: ", target)) return;

    if (!graph.hasUser(source)) {
        std::cout << "  [Loi] User ID " << source << " khong ton tai.\n";
        return;
    }
    if (!graph.hasUser(target)) {
        std::cout << "  [Loi] User ID " << target << " khong ton tai.\n";
        return;
    }

    std::vector<int> path = Algorithms::shortestPath(graph, source, target);

    if (path.empty()) {
        std::cout << "  [!] Khong tim thay duong di giua "
                  << graph.getUser(source).getName() << " va "
                  << graph.getUser(target).getName() << ".\n";
        std::cout << "  Ho khong ket noi trong cung mot thanh phan lien thong.\n";
    } else {
        std::cout << "  Duong di ngan nhat (" << (path.size() - 1) << " buoc):\n\n  ";
        for (size_t i = 0; i < path.size(); i++) {
            if (i > 0) std::cout << " --> ";
            User u = graph.getUser(path[i]);
            std::cout << u.getName() << "(" << path[i] << ")";
        }
        std::cout << "\n";

        // Hỏi có muốn xuất file highlight không
        std::cout << "\n  Xuat file do thi highlight duong di? (y/n): ";
        std::string ans;
        std::getline(std::cin, ans);
        if (ans == "y" || ans == "Y") {
            Visualizer vis;
            vis.highlightPath(graph, path);
        }
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// [9] Hiển thị mạng lưới kết nối
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleDisplayNetwork(const Graph& graph) {
    graph.displayNetwork();
}

// ══════════════════════════════════════════════════════════════════════════════
// [10] Gợi ý kết bạn
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleFriendSuggestions(const Graph& graph) {
    std::cout << "--- GOI Y KET BAN (2-hop BFS) ---\n";

    int userId;
    if (!readInt("  Nhap User ID: ", userId)) return;

    if (!graph.hasUser(userId)) {
        std::cout << "  [Loi] User ID " << userId << " khong ton tai.\n";
        return;
    }

    int topK;
    if (!readInt("  So luong goi y toi da (VD: 5): ", topK)) return;
    if (topK <= 0) {
        std::cout << "  [Loi] Top-K phai lon hon 0.\n";
        return;
    }

    std::vector<std::pair<int, int>> suggestions =
        Algorithms::friendSuggestions(graph, userId, topK);

    User user = graph.getUser(userId);
    std::cout << "\n  Goi y ket ban cho " << user.getName()
              << " (ID: " << userId << "):\n";

    if (suggestions.empty()) {
        std::cout << "  [!] Khong tim thay goi y nao.\n";
    } else {
        std::cout << "  " << std::string(50, '-') << "\n";
        std::cout << "  #   Ten                        Ban chung\n";
        std::cout << "  " << std::string(50, '-') << "\n";

        int rank = 1;
        for (const auto& s : suggestions) {
            int candidateId = s.first;
            int mutualCount = s.second;

            if (graph.hasUser(candidateId)) {
                User candidate = graph.getUser(candidateId);
                // Căn chỉnh cột
                std::string nameField = candidate.getName() + " (" + std::to_string(candidateId) + ")";
                // Pad đến 30 ký tự
                while (nameField.size() < 30) nameField += " ";

                std::cout << "  " << rank << ".  "
                          << nameField << mutualCount << "\n";
            }
            rank++;
        }
        std::cout << "  " << std::string(50, '-') << "\n";
    }
}

// ══════════════════════════════════════════════════════════════════════════════
// [11] Trực quan hóa đồ thị
// ══════════════════════════════════════════════════════════════════════════════
void Menu::handleVisualize(const Graph& graph) {
    std::cout << "--- TRUC QUAN HOA DO THI ---\n";
    std::cout << "  [1] Ve toan bo do thi\n";
    std::cout << "  [2] Xuat anh PNG (can Graphviz)\n";
    std::cout << "  [0] Quay lai\n";

    int sub;
    if (!readInt("  Lua chon: ", sub)) return;

    Visualizer vis;

    switch (sub) {
        case 1:
            vis.drawGraph(graph);
            break;
        case 2: {
            std::cout << "  Chon loai:\n";
            std::cout << "    [1] Toan bo mang luoi -> social_network.png\n";
            std::cout << "    [2] Duong di da highlight -> social_network_path.png\n";

            int imgType;
            if (!readInt("  Lua chon: ", imgType)) return;

            if (imgType == 1) {
                // Tạo .dot trước nếu chưa có
                vis.drawGraph(graph);
                vis.exportImage("social_network.png");
            } else if (imgType == 2) {
                vis.exportImage("social_network_path.png");
            } else {
                std::cout << "  [Loi] Lua chon khong hop le.\n";
            }
            break;
        }
        case 0:
            break;
        default:
            std::cout << "  [Loi] Lua chon khong hop le.\n";
            break;
    }
}
