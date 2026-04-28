#include "social_network.h"
#include "file_io.h"

/**
 * Main — Demo mô phỏng mạng xã hội
 * 
 * Load mạng từ file, demo BFS, gợi ý kết bạn, và save
 */
int main() {
    SocialNetwork network;

    cout << "========================================" << endl;
    cout << "   SOCIAL NETWORK SIMULATION" << endl;
    cout << "   BFS Friend Suggestion Demo" << endl;
    cout << "========================================" << endl;
    cout << endl;

    // ======== 1. LOAD DỮ LIỆU ========
    string dataFile = "../data/network.txt";

    cout << "[1] LOAD DU LIEU" << endl;
    if (!FileIO::loadFromFile(network, dataFile)) {
        cerr << "Khong the load du lieu! Kiem tra file: " << dataFile << endl;
        return 1;
    }
    cout << "  So luong user: " << network.getUserCount() << endl;
    cout << "  So luong ket noi: " << network.getEdgeCount() << endl;
    cout << endl;

    // ======== 2. HIỂN THỊ MẠNG ========
    cout << "[2] HIEN THI MANG" << endl;
    network.displayNetwork();
    cout << endl;

    // ======== 3. DEMO BFS ========
    cout << "[3] BFS TRAVERSAL" << endl;
    string startNode = "An";
    cout << "  BFS tu \"" << startNode << "\":" << endl;
    cout << "  ";

    vector<string> bfsResult = network.bfs(startNode);
    for (int i = 0; i < bfsResult.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << bfsResult[i];
    }
    cout << endl;
    cout << "  Tong so node duyet: " << bfsResult.size() << endl;
    cout << endl;

    // ======== 4. GỢI Ý KẾT BẠN ========
    cout << "[4] GOI Y KET BAN" << endl;
    
    // Demo cho nhiều user
    vector<string> demoUsers = {"An", "Binh", "Khoa"};
    for (const string& user : demoUsers) {
        cout << "  Goi y cho \"" << user << "\":" << endl;
        
        vector<pair<string, int>> suggestions = network.suggestFriends(user);
        if (suggestions.empty()) {
            cout << "    (Khong co goi y)" << endl;
        } else {
            for (const auto& s : suggestions) {
                cout << "    -> " << s.first 
                     << " (" << s.second << " ban chung)" << endl;
            }
        }
        cout << endl;
    }

    // ======== 5. DEMO MUTUAL FRIENDS ========
    cout << "[5] BAN CHUNG" << endl;
    cout << "  An & Em: " << network.mutualFriendsCount("An", "Em") 
         << " ban chung" << endl;
    cout << "  An & Giang: " << network.mutualFriendsCount("An", "Giang") 
         << " ban chung" << endl;
    cout << "  Hoa & Linh: " << network.mutualFriendsCount("Hoa", "Linh") 
         << " ban chung" << endl;
    cout << endl;

    // ======== 6. SAVE ========
    cout << "[6] SAVE DU LIEU" << endl;
    string saveFile = "../data/network_saved.txt";
    FileIO::saveToFile(network, saveFile);

    cout << endl;
    cout << "========================================" << endl;
    cout << "   DONE!" << endl;
    cout << "========================================" << endl;

    return 0;
}
