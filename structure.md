```text
social_network_sim/
├── CMakelist.txt                # File cấu hình CMake — định nghĩa cách biên dịch dự án (C++17, liên kết source, tạo executable)
├── data/                        # Thư mục chứa dữ liệu CSV (persistence layer)
│   ├── users.csv                # Dữ liệu người dùng: id, name, age, location (mỗi dòng = 1 user)
│   └── friendships.csv          # Danh sách cạnh (quan hệ bạn bè): user_id_1, user_id_2 (mỗi cạnh ghi 1 lần)
├── include/                     # Thư mục chứa header files — khai báo class/function
│   ├── User.h                   # Khai báo class User — mô hình dữ liệu người dùng (id, name, age, location)
│   ├── Graph.h                  # Khai báo class Graph — đồ thị vô hướng dùng adjacency list (unordered_map + unordered_set)
│   ├── CSVHandler.h             # Khai báo các hàm đọc/ghi CSV — load, save, append, rewrite dữ liệu
│   ├── Algorithms.h             # Khai báo thuật toán đồ thị — BFS, bạn chung, gợi ý kết bạn 2-hop, shortest path
│   ├── Menu.h                   # Khai báo giao diện CLI — menu 12 lựa chọn và các handler tương ứng
│   └── Visualizer.h             # Khai báo module trực quan hóa — vẽ graph, highlight path, xuất ảnh
├── src/                         # Thư mục chứa source files — triển khai (implementation)
│   ├── User.cpp                 # Triển khai class User — constructor, getter/setter, toString() 
│   ├── Graph.cpp                # Triển khai class Graph — thêm/xóa user & edge, truy vấn neighbors, hiển thị mạng lưới
│   ├── CSVHandler.cpp           # Triển khai đọc/ghi CSV — parse file, instant sync (cập nhật file ngay mỗi thao tác)
│   ├── Algorithms.cpp           # Triển khai thuật toán — BFS (queue + visited), mutual friends, friend suggestions, shortest path
│   ├── Menu.cpp                 # Triển khai vòng lặp CLI — hiển thị menu, đọc input, validate, dispatch đến module tương ứng
│   ├── Visualizer.cpp           # Triển khai trực quan hóa — layout nodes, render edges, highlight paths, export image
│   └── main.cpp                 # Entry point — tạo Graph, load CSV, khởi chạy Menu.run() (vòng lặp chính)
├── tests/                       # Thư mục chứa unit tests
│   ├── test_graph.cpp           # Test thao tác Graph — addUser, removeUser, addEdge, removeEdge, hasEdge, getNeighbors
│   └── test_algorithms.cpp      # Test thuật toán — BFS order, mutual friends, friend suggestion ranking, shortest path
├── report/                      # Thư mục chứa báo cáo
│   └── DSA_report.md            # Báo cáo học thuật môn DSA — lý thuyết, thiết kế, phân tích độ phức tạp, kết quả
└── README.md                    # Tài liệu dự án — tổng quan, cấu trúc dữ liệu, thuật toán, hướng dẫn cài đặt & chạy
```
