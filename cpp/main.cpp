#include "social_network.h"
#include "file_io.h"

/**
 * Main — Demo mô phỏng mạng xã hội
 * 
 * Load mạng từ file data, hiển thị, và demo save
 */
int main() {
    SocialNetwork network;

    cout << "=== Social Network Simulation ===" << endl;
    cout << endl;

    // --- Load dữ liệu từ file ---
    string dataFile = "../data/network.txt";

    if (!FileIO::loadFromFile(network, dataFile)) {
        cerr << "Khong the load du lieu! Kiem tra file: " << dataFile << endl;
        return 1;
    }

    cout << endl;
    cout << "So luong user: " << network.getUserCount() << endl;
    cout << "So luong ket noi: " << network.getEdgeCount() << endl;
    cout << endl;

    // --- Hiển thị mạng ---
    network.displayNetwork();

    // --- Demo save ra file mới ---
    string saveFile = "../data/network_saved.txt";
    FileIO::saveToFile(network, saveFile);

    // TODO (Step 3): Demo BFS traversal
    // TODO (Step 3): Demo friend suggestion

    return 0;
}
