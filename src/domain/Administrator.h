#pragma once
#include "Person.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Administrator : public Person
{
private:
    std::string createdDate;

public:
    // Default Constructor
    Administrator() : Person(), createdDate("")
    {
        isActive = true;
    }

    // Parameterized Constructor
    Administrator(int id, const std::string &uname, const std::string &pass, const std::string &fName,
                  const std::string &lName, const std::string &email, const std::string &cDate, bool active)
        : Person(id, uname, pass, fName, lName, email, active), createdDate(cDate) {}

    // Overriding Pure Virtual Function (Satisfies Polymorphism)
    std::string getRole() const override { return "Administrator"; }

    int getAdminId() const { return id; }
    void setAdminId(int adminId) { id = adminId; }

    // Unique Getters/Setters
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
    friend ofstream &writeToFile(ofstream &out, const Administrator &a)
    {
        out << a.id << '|' << a.username << '|' << a.password << '|' << a.firstName << '|'
            << a.lastName << '|' << a.email << '|' << a.isActive << '|' << a.createdDate;
        return out;
    }
    friend ifstream &readFromFile(ifstream &in, Administrator &a)
    {
        string t;
        if (!getline(in, t, '|'))
            return in;
        a.id = stoi(t);
        getline(in, a.username, '|');
        getline(in, a.password, '|');
        getline(in, a.firstName, '|');
        getline(in, a.lastName, '|');
        getline(in, a.email, '|');
        getline(in, t, '|');
        a.isActive = stoi(t);
        getline(in, a.createdDate);
        return in;
    }
};