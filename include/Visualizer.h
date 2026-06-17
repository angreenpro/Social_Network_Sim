#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <vector>
#include <string>
#include "Graph.h"

// Lớp Visualizer trực quan hóa đồ thị mạng xã hội
class Visualizer {
public:
    // Vẽ đồ thị mạng xã hội với các node và edge
    void drawGraph(const Graph& graph);

    // Vẽ đồ thị và highlight một đường đi cụ thể (ví dụ: shortest path)
    void highlightPath(const Graph& graph, const std::vector<int>& path);

    // Lưu đồ thị đã vẽ ra file ảnh
    void exportImage(const std::string& filename);
};

#endif // VISUALIZER_H
