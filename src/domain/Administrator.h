#pragma once
#include "Person.h"
#include <string>
#include <iostream>
#include <fstream>

class Administrator : public Person
{
private:
    std::string createdDate;

public:
    Administrator() : Person(), createdDate("")
    {
        isActive = true;
    }

    Administrator(int id, const std::string &uname, const std::string &pass, const std::string &fName,
                  const std::string &lName, const std::string &email, const std::string &cDate, bool active)
        : Person(id, uname, pass, fName, lName, email, active), createdDate(cDate) {}

    std::string getRole() const override { return "Administrator"; }

    int getAdminId() const { return id; }
    void setAdminId(int adminId) { id = adminId; }

    std::string getCreatedDate() const { return createdDate; }
    void setCreatedDate(const std::string &cDate) { createdDate = cDate; }

    friend std::ostream &operator<<(std::ostream &os, const Administrator &a)
    {
        os << "Admin ID: " << a.getAdminId()
           << " | Name: " << a.getFirstName() << " " << a.getLastName()
           << " | Username: " << a.getUsername()
           << " | Active: " << (a.getIsActive() ? "Yes" : "No");
        return os;
    }

    friend std::ofstream &writeToFile(std::ofstream &out, const Administrator &a)
    {
        out << a.id << '|' << a.username << '|' << a.password << '|' << a.firstName << '|'
            << a.lastName << '|' << a.email << '|' << a.isActive << '|' << a.createdDate;
        return out;
    }

    friend std::ifstream &readFromFile(std::ifstream &in, Administrator &a)
    {
        std::string t;
        if (!std::getline(in, t, '|'))
            return in;
        a.id = std::stoi(t);
        std::getline(in, a.username, '|');
        std::getline(in, a.password, '|');
        std::getline(in, a.firstName, '|');
        std::getline(in, a.lastName, '|');
        std::getline(in, a.email, '|');
        std::getline(in, t, '|');
        a.isActive = std::stoi(t);
        std::getline(in, a.createdDate);
        return in;
    }
};