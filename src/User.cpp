#include "User.h"
#include <string>

// Constructor mặc định
User::User() : id(0), name(""), age(0), location("") {}

// Constructor đầy đủ
User::User(int id, const std::string& name, int age, const std::string& location)
    : id(id), name(name), age(age), location(location) {}

// Getters
int User::getId() const { return id; }
std::string User::getName() const { return name; }
int User::getAge() const { return age; }
std::string User::getLocation() const { return location; }

// Setters
void User::setName(const std::string& name) { this->name = name; }
void User::setAge(int age) { this->age = age; }
void User::setLocation(const std::string& location) { this->location = location; }

// toString
std::string User::toString() const {
    return "ID: " + std::to_string(id) +
           " | Name: " + name +
           " | Age: " + std::to_string(age) +
           " | Location: " + location;
}
