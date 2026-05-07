#pragma once
#include "Person.h"
#include <string>
#include <iostream>
#include <fstream>

class User : public Person
{
private:
    std::string address;
    std::string phone;
    double balance;
    int membershipTypeId;
    std::string registrationDate;
    bool deletionRequested;

public:
    User() : Person(), balance(0.0), membershipTypeId(0), deletionRequested(false) {}

    User(int id, const std::string &uname, const std::string &pass, const std::string &fName,
         const std::string &lName, const std::string &email, const std::string &addr, const std::string &phone,
         double bal, int memTypeId, const std::string &regDate, bool active, bool delRequest = false)
        : Person(id, uname, pass, fName, lName, email, active),
          address(addr), phone(phone), balance(bal), membershipTypeId(memTypeId),
          registrationDate(regDate), deletionRequested(delRequest) {}

    std::string getRole() const override { return "Member"; }

    int getUserId() const { return id; }
    void setUserId(int userId) { id = userId; }

    std::string getAddress() const { return address; }
    std::string getPhone() const { return phone; }
    double getBalance() const { return balance; }
    int getMembershipTypeId() const { return membershipTypeId; }
    std::string getRegistrationDate() const { return registrationDate; }
    bool getDeletionRequested() const { return deletionRequested; }

    void setAddress(const std::string &addr) { address = addr; }
    void setPhone(const std::string &p) { phone = p; }
    void setBalance(double bal) { balance = bal; }
    void setMembershipTypeId(int id) { membershipTypeId = id; }
    void setRegistrationDate(const std::string &date) { registrationDate = date; }
    void setDeletionRequested(bool req) { deletionRequested = req; }

    friend std::ostream &operator<<(std::ostream &os, const User &u)
    {
        os << "User ID: " << u.getUserId()
           << " | Name: " << u.getFirstName() << " " << u.getLastName()
           << " | Username: " << u.getUsername()
           << " | Active: " << (u.getIsActive() ? "Yes" : "SUSPENDED");
        return os;
    }

    friend std::ofstream &writeToFile(std::ofstream &out, const User &u)
    {
        out << u.id << '|' << u.username << '|' << u.password << '|' << u.firstName << '|'
            << u.lastName << '|' << u.email << '|' << u.isActive << '|' << u.address << '|'
            << u.phone << '|' << u.balance << '|' << u.membershipTypeId << '|'
            << u.registrationDate << '|' << u.deletionRequested;
        return out;
    }

    friend std::ifstream &readFromFile(std::ifstream &in, User &u)
    {
        std::string t;
        if (!std::getline(in, t, '|'))
            return in;
        u.id = std::stoi(t);
        std::getline(in, u.username, '|');
        std::getline(in, u.password, '|');
        std::getline(in, u.firstName, '|');
        std::getline(in, u.lastName, '|');
        std::getline(in, u.email, '|');
        std::getline(in, t, '|');
        u.isActive = std::stoi(t);
        std::getline(in, u.address, '|');
        std::getline(in, u.phone, '|');
        std::getline(in, t, '|');
        u.balance = std::stod(t);
        std::getline(in, t, '|');
        u.membershipTypeId = std::stoi(t);
        std::getline(in, u.registrationDate, '|');
        std::getline(in, t);
        u.deletionRequested = std::stoi(t);
        return in;
    }
};