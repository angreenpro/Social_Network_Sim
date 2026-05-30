#ifndef USER_H
#define USER_H

#include <string>

// =============================================================================
// Class User
// Mô hình dữ liệu cho một người dùng trong mạng xã hội.
// Mỗi User có: id (định danh duy nhất), name, age, location.
// Được lưu trữ trong unordered_map<int, User> bên trong Graph.
// =============================================================================
class User {
private:
    int    id;        // Định danh duy nhất của người dùng
    std::string name;      // Tên hiển thị
    int    age;       // Tuổi
    std::string location;  // Thành phố / khu vực

public:
    // -------------------------------------------------------------------------
    // Constructor
    // Khởi tạo một User với đầy đủ thông tin.
    // -------------------------------------------------------------------------
    User(int id, const std::string& name, int age, const std::string& location);

    // Constructor mặc định (cần cho unordered_map)
    User();

    // -------------------------------------------------------------------------
    // Getters — trả về giá trị thuộc tính (const, không thay đổi object)
    // -------------------------------------------------------------------------
    int getId() const;
    std::string getName() const;
    int getAge() const;
    std::string getLocation() const;

    // -------------------------------------------------------------------------
    // Setters — cập nhật giá trị thuộc tính
    // -------------------------------------------------------------------------
    void setName(const std::string& name);
    void setAge(int age);
    void setLocation(const std::string& location);

    // -------------------------------------------------------------------------
    // toString()
    // Trả về chuỗi mô tả người dùng dạng: "ID: 1 | Name: Alice | Age: 22 | Location: Hanoi"
    // Dùng cho hiển thị và debug.
    // -------------------------------------------------------------------------
    std::string toString() const;
};

#endif // USER_H
