# MI3060 - Cấu trúc dữ liệu và giải thuật - Social Network Simulator

> Dự án mô phỏng mạng xã hội thu nhỏ, tập trung vào **Cấu trúc Dữ liệu và Thuật toán (DSA)** — với tính năng chính là xác định *"người quen của người quen"* và đưa ra **gợi ý kết bạn**.

---

## Mục lục

- [Tổng quan](#1-tổng-quan)
- [Cấu trúc dữ liệu](#2-cấu-trúc-dữ-liệu)
  - [Các cấu trúc sử dụng](#21-các-cấu-trúc-dữ-liệu-sử-dụng)
  - [Độ phức tạp không gian](#22-độ-phức-tạp-không-gian)
- [Thuật toán](#3-thuật-toán)
  - [Cách hoạt động](#31-cách-hoạt-động)
  - [Mô phỏng](#32-mô-phỏng)
  - [Độ phức tạp thời gian](#33-độ-phức-tạp-thời-gian)
- [Tính năng chính](#4-tính-năng-chính)
- [Cấu trúc dự án](#5-cấu-trúc-dự-án)
- [Cài đặt & Chạy](#6-cài-đặt--chạy)
- [Định dạng dữ liệu](#7-định-dạng-dữ-liệu-csv)

---

## 1. Tổng quan

Dự án mô phỏng mạng lưới người dùng dưới dạng **đồ thị vô hướng (undirected graph)**, trong đó:

- **Đỉnh (vertex)** — đại diện cho một người dùng (user)
- **Cạnh (edge)** — đại diện cho mối quan hệ bạn bè (friendship)

Mục tiêu là áp dụng các thuật toán đồ thị để phân tích mạng lưới, quản lý dữ liệu và đưa ra các gợi ý kết bạn thông minh dựa trên số lượng **bạn chung**.

---

## 2. Cấu trúc dữ liệu

### 2.1 Các cấu trúc dữ liệu sử dụng

| Cấu trúc | Vai trò | Lý do chọn |
|---|---|---|
| `unordered_map<int, unordered_set<int>>` | Adjacency List — lưu danh sách kề | O(1) average lookup, tiết kiệm bộ nhớ hơn ma trận kề cho đồ thị thưa |
| `unordered_map<int, User>` | Lưu thông tin metadata của từng node | O(1) truy cập theo ID |
| `queue<int>` | Hàng đợi BFS | O(1) push/pop, FIFO đúng thứ tự duyệt |
| `unordered_set<int>` | Tập visited trong BFS | O(1) kiểm tra đã thăm chưa |
| `vector<pair<int,int>>` | Danh sách gợi ý kết bạn | Dễ sort theo số bạn chung giảm dần |

**So sánh Adjacency List vs Adjacency Matrix:**

```text
Adjacency List  → O(V + E) bộ nhớ  — phù hợp đồ thị thưa (mạng xã hội thực tế)
Adjacency Matrix → O(V²) bộ nhớ   — lãng phí khi V lớn, edge ít
```

### 2.2 Độ phức tạp không gian

| Thành phần | Không gian |
|---|---|
| Adjacency List | O(V + E) |
| User metadata map | O(V) |
| BFS visited set | O(V) |
| Gợi ý kết bạn (1 lần) | O(V) |
| **Tổng** | **O(V + E)** |

---

## 3. Thuật toán

### 3.1 Cách hoạt động

**BFS Traversal**

Duyệt toàn bộ đồ thị theo chiều rộng từ một đỉnh bắt đầu, sử dụng `queue` và tập `visited`:

```text
1. Enqueue(start), đánh dấu visited
2. Dequeue node u
3. Với mỗi neighbor v của u chưa visited: enqueue(v), đánh dấu visited
4. Lặp đến khi queue rỗng
```

**Gợi ý kết bạn (2-hop BFS)**

```text
1. Lấy tập bạn bè trực tiếp S1 = neighbors(u)
2. Với mỗi v trong S1, lấy neighbors(v) → hợp thành tập ứng viên S2
3. Loại bỏ u và tất cả v đã có trong S1 khỏi S2
4. Với mỗi ứng viên w trong S2: đếm |neighbors(u) ∩ neighbors(w)| = số bạn chung
5. Sort S2 giảm dần theo số bạn chung → trả về top-k gợi ý
```

**Bạn chung giữa u và v**

```text
mutual = neighbors(u) ∩ neighbors(v)
```

Dùng `set_intersection` hoặc duyệt tập nhỏ hơn, kiểm tra từng phần tử trong tập lớn hơn.

**Shortest Path (BFS)**

BFS trả về đường đi ngắn nhất trong đồ thị không trọng số. Lưu `parent[]` để reconstruct path.

### 3.2 Mô phỏng

Ví dụ đồ thị:

```text
A --- B --- D
|         |
C ------- E
```

Gọi `friendSuggestions(A)`:
- Bạn trực tiếp của A: `{B, C}`
- Bạn của B: `{A, D}` → ứng viên: `D`
- Bạn của C: `{A, E}` → ứng viên: `E`
- Bạn chung của A-D: `{B}` → 1
- Bạn chung của A-E: `{C}` → 1
- Kết quả gợi ý: `[D (1 bạn chung), E (1 bạn chung)]`

### 3.3 Độ phức tạp thời gian

| Thao tác | Độ phức tạp | Ghi chú |
|---|---|---|
| Thêm / xóa user | O(degree(u)) | Xóa phải cập nhật tất cả neighbor |
| Thêm / xóa edge | O(1) avg | unordered_set insert/erase |
| Kiểm tra kết bạn | O(1) avg | Hash set lookup |
| BFS traversal | O(V + E) | Duyệt tất cả đỉnh và cạnh |
| Bạn chung (u, v) | O(min(deg(u), deg(v))) | Duyệt tập nhỏ hơn |
| Gợi ý kết bạn | O(deg² + k log k) | deg² duyệt 2-hop, sort k ứng viên |
| Shortest path (BFS) | O(V + E) | |

---

## 4. Tính năng chính

Hệ thống được thiết kế theo hướng **module hóa**, mỗi tính năng là một module riêng biệt.

### Menu & Tương tác CLI

```text
[1] Thêm người dùng
[2] Xóa người dùng
[3] Thêm mối quan hệ bạn bè
[4] Xóa mối quan hệ bạn bè
[5] Xem số lượng bạn bè
[6] Xem danh sách bạn chung
[7] BFS Traversal từ một user
[8] Tìm đường đi ngắn nhất
[9] Hiển thị mạng lưới kết nối
[10] Gợi ý kết bạn
[11] Trực quan hóa đồ thị
[0] Thoát
```

### Quản lý dữ liệu (File I/O)

- Dữ liệu được lưu dưới định dạng `.csv`
- Load đồ thị từ file khi khởi động
- **Đồng bộ tức thời**: mỗi thao tác thêm/xóa user hoặc friendship đều cập nhật file CSV ngay lập tức

### Thuật toán cốt lõi

- Xác định **bạn chung** giữa hai người dùng
- Xác định **người quen của người quen** (2-hop neighbor)
- Đưa ra danh sách **gợi ý kết bạn** sắp xếp theo độ liên quan (số bạn chung)

### Trực quan hóa

Dự án tích hợp trực quan hóa tự động thông qua **Graphviz** (ngôn ngữ `DOT`):
- **Vẽ mạng lưới:** Tự động gen ra file `.dot` và tạo hình ảnh `.png` cho toàn bộ đồ thị. Các đỉnh (users) có nhiều bạn bè (hub) sẽ được tự động tô màu nổi bật (Đỏ, Cam, Xanh sáng). In thêm biểu đồ phân bố degree bằng ASCII art trên console.
- **Highlight đường đi:** Sau khi tìm được đường đi ngắn nhất, hệ thống tự động xuất riêng đồ thị với đường đi được làm nổi bật (Màu vàng sáng, viền đỏ, nút bắt đầu màu xanh lá, nút kết thúc màu đỏ).
- **Yêu cầu phụ:** Nếu hệ thống của bạn đã cài đặt [Graphviz](https://graphviz.org/download/) (lệnh `dot`), phần mềm sẽ tự động render ra file ảnh `.png`. Nếu không, file `.dot` vẫn được tạo ra và bạn có thể mở nó bằng các công cụ online như GraphvizOnline.

---

## 5. Cấu trúc dự án

```text
social_network_sim/
├── CMakeLists.txt
├── data/
│   ├── users.csv           # id, name, age, location
│   └── friendships.csv     # user_id_1, user_id_2
├── include/
│   ├── User.h
│   ├── Graph.h
│   ├── CSVHandler.h
│   ├── Algorithms.h
│   ├── Menu.h
│   └── Visualizer.h
├── src/
│   ├── main.cpp
│   ├── User.cpp
│   ├── Graph.cpp
│   ├── CSVHandler.cpp
│   ├── Algorithms.cpp
│   ├── Menu.cpp
│   └── Visualizer.cpp
├── tests/
│   ├── test_graph.cpp
│   └── test_algorithms.cpp
└── README.md
```

---

## 6. Cài đặt & Chạy

### Yêu cầu

- C++17 trở lên
- CMake >= 3.16
- g++ hoặc clang++

### Build
Mở terminal (PowerShell hoặc CMD) và chạy:

```bash
# Biên dịch toàn bộ dự án
.\build.bat
```

Sau khi hoàn tất, các file thực thi (`.exe`) sẽ được tạo thẳng ở thư mục gốc.

### Chạy Chương trình Chính

Chương trình chính tự động đọc `data/users.csv` và `data/friendships.csv` khi khởi động.
```bash
# Chạy chương trình tương tác (CLI)
.\social_net.exe
```

### Chạy Unit Tests và Performance Tests

Dự án bao gồm các bộ test để kiểm tra tính đúng đắn và hiệu năng của cấu trúc dữ liệu và thuật toán.

```bash
# Test cấu trúc đồ thị cơ bản
.\test_graph.exe

# Test tính đúng đắn của các thuật toán
.\test_algorithms.exe

# Test hiệu năng với dữ liệu lớn (V=100.000, E=2.000.000)
.\test_performance.exe
```

---

## 7. Định dạng dữ liệu CSV

**`data/users.csv`**

```csv
id,name,age,location
1,Alice,22,Hanoi
2,Bob,24,HCM
3,Charlie,21,Danang
```

**`data/friendships.csv`**

```csv
user_id_1,user_id_2
1,2
1,3
2,3
```

Mỗi cạnh chỉ được lưu một lần (đồ thị vô hướng). Khi thêm cạnh (u, v), file tự động ghi thêm dòng `u,v`.

---

*Dự án thực hiện trong khuôn khổ môn học Cấu trúc Dữ liệu và Thuật toán.*
