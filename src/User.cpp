#include "User.h"
#include <string>

// =============================================================================
// Constructor mặc định
// Cần cho unordered_map<int, User> — map yêu cầu value type có default constructor
// khi truy cập bằng operator[] mà key chưa tồn tại.
// =============================================================================
User::User() : id(0), name(""), age(0), location("") {}

// =============================================================================
// Constructor đầy đủ
// Khởi tạo User với tất cả thông tin — sử dụng initializer list cho hiệu quả
// (tránh default-construct rồi assign lại).
// =============================================================================
User::User(int id, const std::string& name, int age, const std::string& location)
    : id(id), name(name), age(age), location(location) {}

// =============================================================================
// Getters — trả về giá trị thuộc tính (const method, không thay đổi object)
// =============================================================================

int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

int User::getAge() const {
    return age;
}

std::string User::getLocation() const {
    return location;
}

// =============================================================================
// Setters — cập nhật giá trị thuộc tính
// =============================================================================

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setAge(int age) {
    this->age = age;
}

void User::setLocation(const std::string& location) {
    this->location = location;
}

// =============================================================================
// toString()
// Trả về chuỗi mô tả người dùng theo format dễ đọc.
// Dùng cho hiển thị trong menu và debug.
// Ví dụ: "ID: 1 | Name: Nguyen Van An | Age: 22 | Location: Ha Noi"
// =============================================================================
std::string User::toString() const {
    return "ID: " + std::to_string(id) +
           " | Name: " + name +
           " | Age: " + std::to_string(age) +
           " | Location: " + location;
}
