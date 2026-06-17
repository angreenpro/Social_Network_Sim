#ifndef USER_H
#define USER_H

#include <string>

// Lớp User đại diện cho người dùng trong mạng xã hội
class User {
private:
    int id;                // Định danh duy nhất
    std::string name;      // Tên hiển thị
    int age;               // Tuổi
    std::string location;  // Nơi ở

public:
    User(int id, const std::string& name, int age, const std::string& location);
    User();

    // Getters
    int getId() const;
    std::string getName() const;
    int getAge() const;
    std::string getLocation() const;

    // Setters
    void setName(const std::string& name);
    void setAge(int age);
    void setLocation(const std::string& location);

    // Trả về chuỗi mô tả thông tin người dùng
    std::string toString() const;
};

#endif // USER_H
