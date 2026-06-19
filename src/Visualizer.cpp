#include "Visualizer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <iomanip>

// ══════════════════════════════════════════════════════════════════════════════
// Visualizer — Trực quan hóa đồ thị mạng xã hội
//
// Phương pháp: Xuất file .dot (Graphviz DOT language)
// - drawGraph():    Xuất toàn bộ đồ thị ra file .dot và hiển thị tóm tắt console
// - highlightPath(): Xuất đồ thị với đường đi được highlight bằng màu đỏ
// - exportImage():  Gọi lệnh Graphviz `dot` để render ra file ảnh PNG
//
// Nếu hệ thống có cài Graphviz, file .dot sẽ tự động render thành ảnh.
// Nếu không, user vẫn có thể mở file .dot bằng công cụ online.
// ══════════════════════════════════════════════════════════════════════════════

// ── Helper: tạo nội dung DOT cơ bản (không highlight) ───────────────────────
static std::string generateDotContent(const Graph& graph) {
    std::ostringstream dot;

    dot << "graph SocialNetwork {\n";
    dot << "    // Graph attributes\n";
    dot << "    graph [\n";
    dot << "        layout=neato\n";
    dot << "        overlap=false\n";
    dot << "        splines=true\n";
    dot << "        bgcolor=\"#1a1a2e\"\n";
    dot << "        pad=\"0.5\"\n";
    dot << "    ];\n\n";

    dot << "    // Default node style\n";
    dot << "    node [\n";
    dot << "        shape=circle\n";
    dot << "        style=filled\n";
    dot << "        fillcolor=\"#0f3460\"\n";
    dot << "        fontcolor=\"#e0e0e0\"\n";
    dot << "        fontname=\"Arial\"\n";
    dot << "        fontsize=9\n";
    dot << "        color=\"#16213e\"\n";
    dot << "        penwidth=1.5\n";
    dot << "        width=0.6\n";
    dot << "        height=0.6\n";
    dot << "    ];\n\n";

    dot << "    // Default edge style\n";
    dot << "    edge [\n";
    dot << "        color=\"#533483\"\n";
    dot << "        penwidth=1.0\n";
    dot << "    ];\n\n";

    // Nodes
    dot << "    // Nodes\n";
    std::vector<int> ids = graph.getAllUserIds();
    std::sort(ids.begin(), ids.end());

    for (int id : ids) {
        User user = graph.getUser(id);
        int degree = graph.getDegree(id);

        // Tô màu node dựa trên degree (nhiều bạn = nổi bật hơn)
        std::string fillColor = "#0f3460";  // default: xanh đậm
        double nodeWidth = 0.6;

        if (degree >= 8) {
            fillColor = "#e94560";  // Đỏ — hub chính
            nodeWidth = 0.9;
        } else if (degree >= 6) {
            fillColor = "#f5a623";  // Cam — hub phụ
            nodeWidth = 0.8;
        } else if (degree >= 4) {
            fillColor = "#00b4d8";  // Xanh dương — trung bình
            nodeWidth = 0.7;
        }

        dot << "    " << id << " ["
            << "label=\"" << id << "\\n" << user.getName().substr(0, 10) << "\""
            << " fillcolor=\"" << fillColor << "\""
            << " width=" << std::fixed << std::setprecision(1) << nodeWidth
            << " tooltip=\"" << user.getName()
            << " | Age: " << user.getAge()
            << " | " << user.getLocation()
            << " | Friends: " << degree << "\""
            << "];\n";
    }

    // Edges — chỉ ghi mỗi cạnh 1 lần (u < v)
    dot << "\n    // Edges\n";
    for (int u : ids) {
        std::unordered_set<int> neighbors = graph.getNeighbors(u);
        for (int v : neighbors) {
            if (u < v) {
                dot << "    " << u << " -- " << v << ";\n";
            }
        }
    }

    dot << "}\n";
    return dot.str();
}

// ── Helper: tạo nội dung DOT với highlight path ─────────────────────────────
static std::string generateDotWithPath(const Graph& graph, const std::vector<int>& path) {
    // Tạo set các node và edge thuộc path để tra cứu nhanh
    std::unordered_set<int> pathNodes(path.begin(), path.end());
    std::unordered_set<std::string> pathEdges;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        int u = std::min(path[i], path[i + 1]);
        int v = std::max(path[i], path[i + 1]);
        pathEdges.insert(std::to_string(u) + "-" + std::to_string(v));
    }

    std::ostringstream dot;

    dot << "graph SocialNetwork {\n";
    dot << "    graph [\n";
    dot << "        layout=neato\n";
    dot << "        overlap=false\n";
    dot << "        splines=true\n";
    dot << "        bgcolor=\"#1a1a2e\"\n";
    dot << "        pad=\"0.5\"\n";
    dot << "    ];\n\n";

    dot << "    node [\n";
    dot << "        shape=circle\n";
    dot << "        style=filled\n";
    dot << "        fillcolor=\"#0f3460\"\n";
    dot << "        fontcolor=\"#e0e0e0\"\n";
    dot << "        fontname=\"Arial\"\n";
    dot << "        fontsize=9\n";
    dot << "        color=\"#16213e\"\n";
    dot << "        penwidth=1.5\n";
    dot << "        width=0.6\n";
    dot << "        height=0.6\n";
    dot << "    ];\n\n";

    dot << "    edge [\n";
    dot << "        color=\"#533483\"\n";
    dot << "        penwidth=1.0\n";
    dot << "    ];\n\n";

    // Nodes
    std::vector<int> ids = graph.getAllUserIds();
    std::sort(ids.begin(), ids.end());

    for (int id : ids) {
        User user = graph.getUser(id);
        bool onPath = pathNodes.count(id) > 0;

        // Node thuộc path → highlight vàng sáng, viền đỏ
        std::string fillColor = onPath ? "#f1c40f" : "#0f3460";
        std::string fontColor = onPath ? "#1a1a2e" : "#e0e0e0";
        std::string borderColor = onPath ? "#e94560" : "#16213e";
        double penWidth = onPath ? 3.0 : 1.5;
        double nodeWidth = onPath ? 0.85 : 0.6;

        // Đánh dấu node start / end
        if (!path.empty() && id == path.front()) {
            fillColor = "#2ecc71";  // Xanh lá — điểm bắt đầu
            nodeWidth = 0.95;
        } else if (!path.empty() && id == path.back()) {
            fillColor = "#e74c3c";  // Đỏ — điểm kết thúc
            fontColor = "#ffffff";
            nodeWidth = 0.95;
        }

        dot << "    " << id << " ["
            << "label=\"" << id << "\\n" << user.getName().substr(0, 10) << "\""
            << " fillcolor=\"" << fillColor << "\""
            << " fontcolor=\"" << fontColor << "\""
            << " color=\"" << borderColor << "\""
            << " penwidth=" << std::fixed << std::setprecision(1) << penWidth
            << " width=" << nodeWidth
            << "];\n";
    }

    // Edges
    dot << "\n";
    for (int u : ids) {
        std::unordered_set<int> neighbors = graph.getNeighbors(u);
        for (int v : neighbors) {
            if (u < v) {
                std::string edgeKey = std::to_string(u) + "-" + std::to_string(v);
                bool onPath = pathEdges.count(edgeKey) > 0;

                if (onPath) {
                    dot << "    " << u << " -- " << v
                        << " [color=\"#e74c3c\" penwidth=3.0 style=bold];\n";
                } else {
                    dot << "    " << u << " -- " << v << ";\n";
                }
            }
        }
    }

    dot << "}\n";
    return dot.str();
}

// ══════════════════════════════════════════════════════════════════════════════
// Public API
// ══════════════════════════════════════════════════════════════════════════════

// Vẽ toàn bộ đồ thị ra file DOT + hiển thị summary trên console
void Visualizer::drawGraph(const Graph& graph) {
    if (graph.getUserCount() == 0) {
        std::cout << "  [Khong co du lieu de ve do thi.]\n";
        return;
    }

    std::string dotContent = generateDotContent(graph);

    // Ghi ra file .dot
    std::string dotFile = "social_network.dot";
    std::ofstream file(dotFile);
    if (!file.is_open()) {
        std::cerr << "  [Loi] Khong the tao file: " << dotFile << "\n";
        return;
    }
    file << dotContent;
    file.close();

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "            DO THI — GRAPHVIZ VISUALIZATION                 \n";
    std::cout << "============================================================\n";
    std::cout << "  Da xuat file:  " << dotFile << "\n";
    std::cout << "  So node:       " << graph.getUserCount() << "\n";
    std::cout << "  So edge:       " << graph.getEdgeCount() << "\n";
    std::cout << "------------------------------------------------------------\n";

    // Hiển thị bảng thống kê degree distribution
    std::cout << "  THONG KE DEGREE (so ban be):\n";
    std::vector<int> ids = graph.getAllUserIds();
    std::sort(ids.begin(), ids.end());

    int maxDeg = 0, minDeg = graph.getUserCount();
    int totalDeg = 0;
    std::unordered_map<int, int> degreeHist;  // degree → count

    for (int id : ids) {
        int d = graph.getDegree(id);
        maxDeg = std::max(maxDeg, d);
        minDeg = std::min(minDeg, d);
        totalDeg += d;
        degreeHist[d]++;
    }

    double avgDeg = ids.empty() ? 0.0 : static_cast<double>(totalDeg) / ids.size();

    std::cout << "    Min: " << minDeg
              << "  |  Max: " << maxDeg
              << "  |  Avg: " << std::fixed << std::setprecision(1) << avgDeg << "\n";

    // Histogram dạng bar chart ASCII
    std::cout << "\n  PHAN BO DEGREE:\n";
    std::vector<std::pair<int, int>> histSorted(degreeHist.begin(), degreeHist.end());
    std::sort(histSorted.begin(), histSorted.end());

    int maxCount = 0;
    for (const auto& p : histSorted) {
        maxCount = std::max(maxCount, p.second);
    }

    for (const auto& p : histSorted) {
        int barLen = (maxCount > 0) ? (p.second * 30 / maxCount) : 0;
        std::cout << "    deg " << std::setw(2) << p.first << " |";
        for (int i = 0; i < barLen; i++) std::cout << "█";
        std::cout << " " << p.second << "\n";
    }

    std::cout << "------------------------------------------------------------\n";
    std::cout << "  Huong dan render anh:\n";
    std::cout << "    dot -Tpng " << dotFile << " -o social_network.png\n";
    std::cout << "    (Can cai dat Graphviz: https://graphviz.org/download/)\n";
    std::cout << "============================================================\n\n";
}

// Vẽ đồ thị với highlight đường đi
void Visualizer::highlightPath(const Graph& graph, const std::vector<int>& path) {
    if (graph.getUserCount() == 0) {
        std::cout << "  [Khong co du lieu de ve do thi.]\n";
        return;
    }

    if (path.empty()) {
        std::cout << "  [Duong di rong — khong co gi de highlight.]\n";
        return;
    }

    std::string dotContent = generateDotWithPath(graph, path);

    std::string dotFile = "social_network_path.dot";
    std::ofstream file(dotFile);
    if (!file.is_open()) {
        std::cerr << "  [Loi] Khong the tao file: " << dotFile << "\n";
        return;
    }
    file << dotContent;
    file.close();

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "         DO THI — HIGHLIGHT DUONG DI                        \n";
    std::cout << "============================================================\n";
    std::cout << "  Da xuat file:  " << dotFile << "\n";
    std::cout << "  Duong di:      ";
    for (size_t i = 0; i < path.size(); i++) {
        if (i > 0) std::cout << " -> ";
        User user = graph.getUser(path[i]);
        std::cout << user.getName() << "(" << path[i] << ")";
    }
    std::cout << "\n";
    std::cout << "  Do dai:        " << (path.size() - 1) << " buoc\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "  Chu thich mau:\n";
    std::cout << "    \033[32m● Xanh la\033[0m  = Diem bat dau\n";
    std::cout << "    \033[31m● Do\033[0m       = Diem ket thuc\n";
    std::cout << "    \033[33m● Vang\033[0m     = Node tren duong di\n";
    std::cout << "    \033[35m— Tim\033[0m      = Cac canh khac\n";
    std::cout << "    \033[31m— Do dam\033[0m   = Canh thuoc duong di\n";
    std::cout << "------------------------------------------------------------\n";
    std::cout << "  Huong dan render anh:\n";
    std::cout << "    dot -Tpng " << dotFile << " -o social_network_path.png\n";
    std::cout << "============================================================\n\n";
}

// Xuất file ảnh từ file .dot bằng lệnh Graphviz
void Visualizer::exportImage(const std::string& filename) {
    // Xác định file .dot input dựa trên tên output
    std::string dotFile;
    if (filename.find("path") != std::string::npos) {
        dotFile = "social_network_path.dot";
    } else {
        dotFile = "social_network.dot";
    }

    // Kiểm tra file .dot tồn tại
    std::ifstream checkFile(dotFile);
    if (!checkFile.is_open()) {
        std::cerr << "  [Loi] Chua co file " << dotFile
                  << ". Hay chay drawGraph() hoac highlightPath() truoc.\n";
        return;
    }
    checkFile.close();

    // Xây dựng lệnh Graphviz: dot -Tpng input.dot -o output.png
    std::string command = "dot -Tpng " + dotFile + " -o " + filename;

    std::cout << "  Dang render: " << command << "\n";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "  [OK] Da xuat anh thanh cong: " << filename << "\n";
    } else {
        std::cerr << "  [Loi] Khong the render anh. Kiem tra Graphviz da cai dat chua.\n";
        std::cerr << "  Tai Graphviz: https://graphviz.org/download/\n";
        std::cerr << "  Hoac dung online: https://dreampuf.github.io/GraphvizOnline/\n";
        std::cerr << "  File .dot van co san tai: " << dotFile << "\n";
    }
}
