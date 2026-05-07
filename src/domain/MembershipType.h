#pragma once
#include <string>
#include <iostream>
#include<fstream>
using namespace std;
class MembershipType
{
private:
    int membershipTypeId;
    std::string membershipName;
    int durationDays;
    double price;
    int maxBorrowingLimit;
    int borrowingDurationDays;
    double finePerDay;
    std::string description;

public:
    MembershipType() : membershipTypeId(0), durationDays(0), price(0.0), maxBorrowingLimit(0),
                       borrowingDurationDays(0), finePerDay(0.0) {}

    MembershipType(int id, const std::string &name, int duration, double price, int maxLimit,
                   int borrowDuration, double fine, const std::string &desc)
        : membershipTypeId(id), membershipName(name), durationDays(duration), price(price),
          maxBorrowingLimit(maxLimit), borrowingDurationDays(borrowDuration), finePerDay(fine), description(desc) {}

    // Getters
    int getMembershipTypeId() const { return membershipTypeId; }
    std::string getMembershipName() const { return membershipName; }
    int getDurationDays() const { return durationDays; }
    double getPrice() const { return price; }
    int getMaxBorrowingLimit() const { return maxBorrowingLimit; }
    int getBorrowingDurationDays() const { return borrowingDurationDays; }
    double getFinePerDay() const { return finePerDay; }
    std::string getDescription() const { return description; }

    // Setters
    void setMembershipTypeId(int id) { membershipTypeId = id; }
    void setMembershipName(const std::string &name) { membershipName = name; }
    void setDurationDays(int days) { durationDays = days; }
    void setPrice(double p) { price = p; }
    void setMaxBorrowingLimit(int limit) { maxBorrowingLimit = limit; }
    void setBorrowingDurationDays(int days) { borrowingDurationDays = days; }
    void setFinePerDay(double fine) { finePerDay = fine; }
    void setDescription(const std::string &desc) { description = desc; }

    friend std::ostream &operator<<(std::ostream &os, const MembershipType &t)
    {
        os << "ID: " << t.getMembershipTypeId()
           << " | Name: " << t.getMembershipName()
           << " | Price: $" << t.getPrice()
           << " | Max Borrow: " << t.getMaxBorrowingLimit() << " items";
        return os;
    }
    friend ofstream &writeToFile(ofstream &out, const MembershipType &m)
    {
        out << m.membershipTypeId << '|' << m.membershipName << '|' << m.durationDays << '|'
            << m.price << '|' << m.maxBorrowingLimit << '|' << m.borrowingDurationDays << '|'
            << m.finePerDay << '|' << m.description;
        return out;
    }
    friend ifstream &readFromFile(ifstream &in, MembershipType &m)
    {
        string t;
        if (!getline(in, t, '|'))
            return in;
        m.membershipTypeId = stoi(t);
        getline(in, m.membershipName, '|');
        getline(in, t, '|');
        m.durationDays = stoi(t);
        getline(in, t, '|');
        m.price = stod(t);
        getline(in, t, '|');
        m.maxBorrowingLimit = stoi(t);
        getline(in, t, '|');
        m.borrowingDurationDays = stoi(t);
        getline(in, t, '|');
        m.finePerDay = stod(t);
        getline(in, m.description);
        return in;
    }
};