#pragma once
#include <string>
#include <iostream>
#include <fstream>

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

    int getMembershipTypeId() const { return membershipTypeId; }
    std::string getMembershipName() const { return membershipName; }
    int getDurationDays() const { return durationDays; }
    double getPrice() const { return price; }
    int getMaxBorrowingLimit() const { return maxBorrowingLimit; }
    int getBorrowingDurationDays() const { return borrowingDurationDays; }
    double getFinePerDay() const { return finePerDay; }
    std::string getDescription() const { return description; }

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

    friend std::ofstream &writeToFile(std::ofstream &out, const MembershipType &m)
    {
        out << m.membershipTypeId << '|' << m.membershipName << '|' << m.durationDays << '|'
            << m.price << '|' << m.maxBorrowingLimit << '|' << m.borrowingDurationDays << '|'
            << m.finePerDay << '|' << m.description;
        return out;
    }

    friend std::ifstream &readFromFile(std::ifstream &in, MembershipType &m)
    {
        std::string t;
        if (!std::getline(in, t, '|'))
            return in;
        m.membershipTypeId = std::stoi(t);
        std::getline(in, m.membershipName, '|');
        std::getline(in, t, '|');
        m.durationDays = std::stoi(t);
        std::getline(in, t, '|');
        m.price = std::stod(t);
        std::getline(in, t, '|');
        m.maxBorrowingLimit = std::stoi(t);
        std::getline(in, t, '|');
        m.borrowingDurationDays = std::stoi(t);
        std::getline(in, t, '|');
        m.finePerDay = std::stod(t);
        std::getline(in, m.description);
        return in;
    }
};