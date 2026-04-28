#ifndef FILE_IO_H
#define FILE_IO_H

#include "social_network.h"
#include <fstream>
#include <sstream>

/**
 * FileIO — Đọc/ghi dữ liệu mạng xã hội từ/ra file
 * 
 * Format file:
 *   # comment lines (bỏ qua)
 *   USERS          ← bắt đầu section users
 *   <tên user>     ← mỗi dòng 1 user
 *   ...
 *   FRIENDSHIPS    ← bắt đầu section friendships
 *   <user1> <user2> ← mỗi dòng 1 cặp bạn bè
 *   ...
 * 
 * Độ phức tạp:
 *   - saveToFile: O(V + E) — duyệt tất cả user và cạnh
 *   - loadFromFile: O(V + E) — đọc tất cả user và cạnh
 */
class FileIO {
public:

    /**
     * Lưu mạng xã hội ra file
     * @param network - mạng cần lưu
     * @param filename - đường dẫn file output
     * @return true nếu lưu thành công
     */
    static bool saveToFile(const SocialNetwork& network, const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "[ERROR] Khong the mo file de ghi: " << filename << endl;
            return false;
        }

        // Header comment
        file << "# Social Network Data" << endl;
        file << "# Saved automatically" << endl;
        file << endl;

        // === USERS section ===
        file << "USERS" << endl;
        vector<string> users = network.getAllUsers();

        // Sắp xếp để output nhất quán
        sort(users.begin(), users.end());

        for (const string& user : users) {
            file << user << endl;
        }
        file << endl;

        // === FRIENDSHIPS section ===
        file << "FRIENDSHIPS" << endl;

        // Dùng set để tránh ghi trùng cạnh (vì đồ thị vô hướng: A-B = B-A)
        // Chỉ ghi cặp mà user1 < user2 (theo thứ tự alphabet)
        for (const string& user : users) {
            vector<string> friends = network.getFriends(user);
            sort(friends.begin(), friends.end());

            for (const string& friendName : friends) {
                // Chỉ ghi khi user < friendName để tránh trùng
                if (user < friendName) {
                    file << user << " " << friendName << endl;
                }
            }
        }

        file.close();
        cout << "[OK] Da luu mang vao file: " << filename << endl;
        return true;
    }

    /**
     * Đọc mạng xã hội từ file
     * @param network - mạng cần nạp dữ liệu vào (sẽ xóa dữ liệu cũ)
     * @param filename - đường dẫn file input
     * @return true nếu đọc thành công
     */
    static bool loadFromFile(SocialNetwork& network, const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "[ERROR] Khong the mo file de doc: " << filename << endl;
            return false;
        }

        // Xóa dữ liệu cũ trước khi nạp
        network.clear();

        string line;
        string currentSection = "";  // "USERS" hoặc "FRIENDSHIPS"

        int userCount = 0;
        int edgeCount = 0;

        while (getline(file, line)) {
            // Bỏ qua dòng trống
            if (line.empty()) continue;

            // Bỏ qua comment (dòng bắt đầu bằng #)
            if (line[0] == '#') continue;

            // Kiểm tra section header
            if (line == "USERS") {
                currentSection = "USERS";
                continue;
            }
            if (line == "FRIENDSHIPS") {
                currentSection = "FRIENDSHIPS";
                continue;
            }

            // Xử lý dữ liệu theo section
            if (currentSection == "USERS") {
                network.addUser(line);
                userCount++;
            }
            else if (currentSection == "FRIENDSHIPS") {
                // Tách user1 và user2 bằng dấu cách
                istringstream iss(line);
                string user1, user2;
                if (iss >> user1 >> user2) {
                    network.addFriendship(user1, user2);
                    edgeCount++;
                } else {
                    cerr << "[WARNING] Dong khong hop le: " << line << endl;
                }
            }
        }

        file.close();
        cout << "[OK] Da doc " << userCount << " users, " 
             << edgeCount << " friendships tu file: " << filename << endl;
        return true;
    }
};

#endif // FILE_IO_H
