# 🌐 Social Network Simulation

> Mô phỏng mạng xã hội: Xác định "người quen của người quen" và gợi ý kết bạn

## 📌 Mô tả

Dự án DSA mô phỏng mạng xã hội bằng đồ thị (Graph), sử dụng thuật toán **BFS (Breadth-First Search)** để:
- Duyệt mạng lưới quan hệ bạn bè
- Xác định "bạn của bạn" (depth 2)
- Gợi ý kết bạn dựa trên số bạn chung (mutual friends)
- Lưu/đọc dữ liệu mạng từ file

## 🏗️ Cấu trúc dự án

```
Social Media Sim/
├── cpp/                        # C++ Core
│   ├── social_network.h        # Class SocialNetwork — Graph + BFS + Suggestion
│   ├── file_io.h               # Class FileIO — Save/Load mạng từ file
│   └── main.cpp                # Demo & console output
├── data/                       # Dữ liệu mạng
│   └── network.txt             # Mạng mẫu (10 users, 12 friendships)
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
.\main.exe
```

> **Lưu ý:** Phải `cd cpp` trước khi chạy vì chương trình đọc file data từ đường dẫn tương đối `../data/network.txt`.

### Web (Visualization)

Mở file `web/index.html` trực tiếp trên trình duyệt, hoặc dùng Live Server:

```bash
cd web
python -m http.server 8080
# Mở trình duyệt: http://localhost:8080
```

## 📊 Cấu trúc dữ liệu

**Adjacency List** — biểu diễn đồ thị mạng xã hội

| CTDL | Vai trò | Độ phức tạp |
|------|---------|-------------|
| `unordered_map<string, unordered_set<string>>` | Adjacency List (đồ thị) | Truy cập node O(1) |
| `unordered_set<string>` | Tập bạn bè mỗi user | Thêm/xóa/kiểm tra O(1) |
| `queue<string>` | Hàng đợi BFS | Push/pop O(1) |
| `map<string, int>` | Đếm bạn chung (suggestion) | Truy cập O(log n) |

**Vì sao dùng Adjacency List thay vì Adjacency Matrix?**
- Mạng xã hội là **sparse graph** (mỗi người chỉ kết bạn một phần nhỏ)
- Adjacency List: **O(V + E)** bộ nhớ vs Adjacency Matrix: **O(V²)**
- Duyệt bạn bè nhanh hơn: O(degree) vs O(V)

## ⚙️ Thuật toán

### BFS (Breadth-First Search)

- **Mục đích:** Duyệt toàn bộ đồ thị theo từng lớp (level)
- **Độ phức tạp:** O(V + E) thời gian, O(V) không gian
- **Ứng dụng:** Tìm "bạn của bạn" ở depth 2

### Gợi ý kết bạn (Friend Suggestion)

```
suggestFriends(user X):
  Với mỗi bạn F của X (depth 1):
    Với mỗi bạn FF của F (depth 2):
      Nếu FF ≠ X và FF chưa là bạn X:
        FF là ứng viên, F là bạn chung → candidates[FF]++
  Sắp xếp candidates theo số bạn chung giảm dần
```
- **Độ phức tạp:** O(d₁ × d₂), d = degree của các node

### File I/O

- **Format:** Text file với 2 section `USERS` và `FRIENDSHIPS`
- **Save:** O(V + E) — ghi tất cả users và edges (tránh trùng bằng điều kiện u < v)
- **Load:** O(V + E) — đọc từng dòng, parse theo section

## 📋 Output mẫu

```
========================================
   SOCIAL NETWORK SIMULATION
========================================

[2] HIEN THI MANG
--- Mang xa hoi (Adjacency List) ---
  An -> Binh, Cuong, Dung
  Binh -> An, Cuong, Em
  Cuong -> An, Binh, Phuc
  ...

[3] BFS TRAVERSAL
  BFS tu "An":
  An -> Binh -> Cuong -> Dung -> Em -> Phuc -> Giang -> Hoa -> Khoa -> Linh

[4] GOI Y KET BAN
  Goi y cho "An":
    -> Em (1 ban chung)
    -> Giang (1 ban chung)
    -> Phuc (1 ban chung)
```

## 📁 Data Format

File `data/network.txt`:
```
# Comment (bỏ qua)
USERS
An
Binh
...

FRIENDSHIPS
An Binh
An Cuong
...
```

## 👤 Tác giả

- Sinh viên — Môn: Cấu trúc dữ liệu và Giải thuật
