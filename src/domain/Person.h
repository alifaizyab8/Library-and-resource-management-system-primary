#pragma once
#include <string>
#include <iostream>
class Person
{
protected:
    int id;
    std::string username;
    std::string password;
    std::string firstName;
    std::string lastName;
    std::string email;
    bool isActive;

public:
    Person() : id(0), isActive(false) {}

    Person(int id, const std::string &uname, const std::string &pass, const std::string &fName,
           const std::string &lName, const std::string &email, bool active)
        : id(id), username(uname), password(pass), firstName(fName), lastName(lName),
          email(email), isActive(active) {}

    virtual ~Person() = default;

    virtual std::string getRole() const = 0;

    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getEmail() const { return email; }
    bool getIsActive() const { return isActive; }

    void setId(int newId) { id = newId; }
    void setUsername(const std::string &uname) { username = uname; }
    void setPassword(const std::string &pass) { password = pass; }
    void setFirstName(const std::string &fName) { firstName = fName; }
    void setLastName(const std::string &lName) { lastName = lName; }
    void setEmail(const std::string &mail) { email = mail; }
    void setIsActive(bool active) { isActive = active; }
};