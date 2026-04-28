#include "social_network.h"
#include "file_io.h"

// ============ HÀM TIỆN ÍCH ============

// Hiển thị menu chính
void showMenu() {
    cout << endl;
    cout << "===== MENU =====" << endl;
    cout << "1. Hien thi mang" << endl;
    cout << "2. Them user" << endl;
    cout << "3. Them ket ban" << endl;
    cout << "4. Xoa user" << endl;
    cout << "5. Xoa ket ban" << endl;
    cout << "6. BFS Traversal" << endl;
    cout << "7. Goi y ket ban" << endl;
    cout << "8. Dem ban chung" << endl;
    cout << "9. Save du lieu" << endl;
    cout << "10. Load du lieu" << endl;
    cout << "0. Thoat" << endl;
    cout << "================" << endl;
    cout << "Lua chon: ";
}

// Thêm user mới
void menuAddUser(SocialNetwork& network) {
    string name;
    cout << "  Nhap ten user moi: ";
    cin >> name;

    if (network.userExists(name)) {
        cout << "  [!] User \"" << name << "\" da ton tai." << endl;
    } else {
        network.addUser(name);
        cout << "  [OK] Da them user \"" << name << "\"." << endl;
    }
}

// Thêm quan hệ bạn bè
void menuAddFriendship(SocialNetwork& network) {
    string u, v;
    cout << "  Nhap user 1: ";
    cin >> u;
    cout << "  Nhap user 2: ";
    cin >> v;

    if (!network.userExists(u)) {
        cout << "  [!] User \"" << u << "\" khong ton tai. Tu dong tao moi." << endl;
    }
    if (!network.userExists(v)) {
        cout << "  [!] User \"" << v << "\" khong ton tai. Tu dong tao moi." << endl;
    }
    if (network.areFriends(u, v)) {
        cout << "  [!] \"" << u << "\" va \"" << v << "\" da la ban." << endl;
        return;
    }

    network.addFriendship(u, v);
    cout << "  [OK] Da ket ban \"" << u << "\" <-> \"" << v << "\"." << endl;
}

// Xóa user
void menuRemoveUser(SocialNetwork& network) {
    string name;
    cout << "  Nhap ten user can xoa: ";
    cin >> name;

    if (!network.userExists(name)) {
        cout << "  [!] User \"" << name << "\" khong ton tai." << endl;
    } else {
        network.removeUser(name);
        cout << "  [OK] Da xoa user \"" << name << "\" va tat ca ket noi." << endl;
    }
}

// Xóa quan hệ bạn bè
void menuRemoveFriendship(SocialNetwork& network) {
    string u, v;
    cout << "  Nhap user 1: ";
    cin >> u;
    cout << "  Nhap user 2: ";
    cin >> v;

    if (!network.areFriends(u, v)) {
        cout << "  [!] \"" << u << "\" va \"" << v << "\" khong phai ban." << endl;
    } else {
        network.removeFriendship(u, v);
        cout << "  [OK] Da huy ket ban \"" << u << "\" <-> \"" << v << "\"." << endl;
    }
}

// BFS traversal
void menuBFS(const SocialNetwork& network) {
    string start;
    cout << "  Nhap node bat dau BFS: ";
    cin >> start;

    if (!network.userExists(start)) {
        cout << "  [!] User \"" << start << "\" khong ton tai." << endl;
        return;
    }

    vector<string> result = network.bfs(start);
    cout << "  BFS tu \"" << start << "\":" << endl;
    cout << "  ";
    for (int i = 0; i < result.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << result[i];
    }
    cout << endl;
    cout << "  Tong node duyet: " << result.size() << endl;
}

// Gợi ý kết bạn
void menuSuggestFriends(const SocialNetwork& network) {
    string user;
    cout << "  Nhap ten user: ";
    cin >> user;

    if (!network.userExists(user)) {
        cout << "  [!] User \"" << user << "\" khong ton tai." << endl;
        return;
    }

    vector<pair<string, int>> suggestions = network.suggestFriends(user);
    cout << "  Goi y ket ban cho \"" << user << "\":" << endl;

    if (suggestions.empty()) {
        cout << "    (Khong co goi y)" << endl;
    } else {
        for (const auto& s : suggestions) {
            cout << "    -> " << s.first 
                 << " (" << s.second << " ban chung)" << endl;
        }
    }
}

// Đếm bạn chung
void menuMutualFriends(const SocialNetwork& network) {
    string u, v;
    cout << "  Nhap user 1: ";
    cin >> u;
    cout << "  Nhap user 2: ";
    cin >> v;

    if (!network.userExists(u) || !network.userExists(v)) {
        cout << "  [!] User khong ton tai." << endl;
        return;
    }

    int count = network.mutualFriendsCount(u, v);
    cout << "  \"" << u << "\" & \"" << v << "\": " << count << " ban chung" << endl;

    // Hiển thị danh sách bạn chung
    if (count > 0) {
        vector<string> friendsU = network.getFriends(u);
        vector<string> friendsV = network.getFriends(v);
        cout << "  Danh sach: ";
        bool first = true;
        for (const string& f : friendsU) {
            for (const string& g : friendsV) {
                if (f == g) {
                    if (!first) cout << ", ";
                    cout << f;
                    first = false;
                }
            }
        }
        cout << endl;
    }
}

// Save dữ liệu
void menuSave(const SocialNetwork& network) {
    string filename;
    cout << "  Nhap ten file (Enter = network_saved.txt): ";
    cin.ignore();
    getline(cin, filename);

    if (filename.empty()) {
        filename = "../data/network_saved.txt";
    } else {
        filename = "../data/" + filename;
    }

    FileIO::saveToFile(network, filename);
}

// Load dữ liệu
void menuLoad(SocialNetwork& network) {
    string filename;
    cout << "  Nhap ten file (Enter = network.txt): ";
    cin.ignore();
    getline(cin, filename);

    if (filename.empty()) {
        filename = "../data/network.txt";
    } else {
        filename = "../data/" + filename;
    }

    FileIO::loadFromFile(network, filename);
    cout << "  Mang hien tai: " << network.getUserCount() << " users, " 
         << network.getEdgeCount() << " connections." << endl;
}

// ============ MAIN ============

int main() {
    SocialNetwork network;
    string dataFile = "../data/network.txt";

    cout << "Mo phong mang xa hoi: Xac dinh nguoi quen cua nguoi quen (BFS) va goi y ket ban." << endl;
    cout << endl;

    // Auto-load dữ liệu ban đầu
    cout << "[Auto-load] ";
    if (FileIO::loadFromFile(network, dataFile)) {
        cout << "Mang: " << network.getUserCount() << " users, " 
             << network.getEdgeCount() << " connections." << endl;
    } else {
        cout << "Khong tim thay file data. Bat dau voi mang rong." << endl;
    }

    // Vòng lặp menu
    int choice;
    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                network.displayNetwork();
                break;
            case 2:
                menuAddUser(network);
                break;
            case 3:
                menuAddFriendship(network);
                break;
            case 4:
                menuRemoveUser(network);
                break;
            case 5:
                menuRemoveFriendship(network);
                break;
            case 6:
                menuBFS(network);
                break;
            case 7:
                menuSuggestFriends(network);
                break;
            case 8:
                menuMutualFriends(network);
                break;
            case 9:
                menuSave(network);
                break;
            case 10:
                menuLoad(network);
                break;
            case 0:
                cout << "Tam biet!" << endl;
                break;
            default:
                cout << "  [!] Lua chon khong hop le." << endl;
        }
    } while (choice != 0);

    return 0;
}
