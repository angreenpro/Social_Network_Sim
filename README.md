# 🌐 Social Network Simulation

> Mô phỏng mạng xã hội: Xác định "người quen của người quen" và gợi ý kết bạn

## 📌 Mô tả

Dự án DSA mô phỏng mạng xã hội bằng đồ thị (Graph), sử dụng thuật toán **BFS (Breadth-First Search)** để:
- Duyệt mạng lưới quan hệ bạn bè
- Xác định "bạn của bạn" (depth 2)
- Gợi ý kết bạn dựa trên số bạn chung (mutual friends)

## 🏗️ Cấu trúc dự án

```
Social Media Sim/
├── cpp/                        # C++ Core
│   ├── social_network.h        # Graph data structure + BFS + Friend suggestion
│   └── main.cpp                # Demo & console output
├── web/                        # Web Visualization
│   ├── index.html              # Main page
│   ├── style.css               # Styling (dark theme)
│   └── app.js                  # Graph rendering + BFS animation
├── .gitignore
└── README.md
```

## 🚀 Hướng dẫn chạy

### C++ (Console)

```bash
cd cpp
g++ -o main main.cpp -std=c++17
./main
```

### Web (Visualization)

Mở file `web/index.html` trực tiếp trên trình duyệt, hoặc dùng Live Server trong VS Code.

## 📊 Cấu trúc dữ liệu

| CTDL | Vai trò | Độ phức tạp |
|------|---------|-------------|
| `unordered_map<string, unordered_set<string>>` | Adjacency List | Thêm/xóa cạnh O(1) |
| `unordered_set<string>` | Tập bạn bè mỗi user | Kiểm tra bạn bè O(1) |
| `queue<string>` | Hàng đợi BFS | Push/pop O(1) |

## ⚙️ Thuật toán

- **BFS Traversal**: O(V + E) — duyệt toàn bộ đồ thị
- **Friend Suggestion**: BFS depth-2, xếp hạng theo số bạn chung

## 👤 Tác giả

- Sinh viên — Môn: Cấu trúc dữ liệu và Giải thuật
