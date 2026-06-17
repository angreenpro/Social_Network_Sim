#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <vector>
#include <string>
#include "Graph.h"

// =============================================================================
// Class Visualizer
// Module trực quan hóa đồ thị mạng xã hội.
//
// Cung cấp khả năng render đồ thị dưới dạng hình ảnh:
//   - Nodes (hình tròn) đại diện cho users
//   - Edges (đường thẳng) đại diện cho friendships
//   - Hỗ trợ highlight paths (BFS, shortest path) bằng màu khác biệt
//   - Xuất kết quả ra file ảnh (PNG)
//
// Layout: Có thể dùng circular layout hoặc force-directed layout
// để sắp xếp vị trí các node sao cho dễ nhìn.
// =============================================================================
class Visualizer {
public:
    // -------------------------------------------------------------------------
    // drawGraph()
    // Render toàn bộ đồ thị với nodes và edges.
    //   - Gán vị trí cho mỗi node (circular/force-directed layout)
    //   - Vẽ các cạnh nối giữa nodes
    //   - Gắn label cho mỗi node (tên user / ID)
    // -------------------------------------------------------------------------
    void drawGraph(const Graph& graph);

    // -------------------------------------------------------------------------
    // highlightPath()
    // Vẽ đồ thị với một đường đi cụ thể được highlight bằng màu khác.
    // Hữu ích khi trực quan hóa kết quả BFS traversal hoặc shortest path.
    //
    // path: vector<int> chứa danh sách user ID theo thứ tự đường đi
    // -------------------------------------------------------------------------
    void highlightPath(const Graph& graph, const std::vector<int>& path);

    // -------------------------------------------------------------------------
    // exportImage()
    // Lưu đồ thị đã render ra file ảnh (ví dụ: PNG).
    //
    // filename: đường dẫn file output (ví dụ: "output/graph.png")
    // -------------------------------------------------------------------------
    void exportImage(const std::string& filename);
};

#endif // VISUALIZER_H
