#pragma once
#include <string>
#include <iostream>
#include <fstream>

class BorrowingHistory
{
private:
    int historyId;
    int userId;
    int resourceId;
    std::string issueDate;
    std::string dueDate;
    std::string returnDate;
    double fineAmount;

public:
    BorrowingHistory()
        : historyId(0), userId(0), resourceId(0), issueDate(""),
          dueDate(""), returnDate(""), fineAmount(0.0) {}

    BorrowingHistory(int userId, int resourceId, std::string issueDate,
                     std::string dueDate, std::string returnDate, double fine)
        : historyId(0), userId(userId), resourceId(resourceId),
          issueDate(issueDate), dueDate(dueDate), returnDate(returnDate),
          fineAmount(fine) {}

    int getId() const { return historyId; }
    int getUserId() const { return userId; }
    int getResourceId() const { return resourceId; }
    std::string getIssueDate() const { return issueDate; }
    std::string getDueDate() const { return dueDate; }
    std::string getReturnDate() const { return returnDate; }
    double getFineAmount() const { return fineAmount; }

    void setId(int id) { historyId = id; }
    void setReturnDate(const std::string &date) { returnDate = date; }
    void setFineAmount(double fine) { fineAmount = fine; }

    friend std::ostream &operator<<(std::ostream &out, const BorrowingHistory &h)
    {
        out << "Record ID: " << h.historyId << " | Resource ID: " << h.resourceId
            << " | Issued: " << h.issueDate << " | Due: " << h.dueDate << " | Returned: "
            << (h.returnDate.empty() ? "Pending" : h.returnDate)
            << " | Fine: $" << h.fineAmount;
        return out;
    }

    friend std::ofstream &writeToFile(std::ofstream &out, const BorrowingHistory &b)
    {
        out << b.historyId << '|' << b.userId << '|' << b.resourceId << '|'
            << b.issueDate << '|' << b.dueDate << '|' << b.returnDate << '|' << b.fineAmount;
        return out;
    }

    friend std::ifstream &readFromFile(std::ifstream &in, BorrowingHistory &b)
    {
        std::string t;
        if (!std::getline(in, t, '|'))
            return in;
        b.historyId = std::stoi(t);
        std::getline(in, t, '|');
        b.userId = std::stoi(t);
        std::getline(in, t, '|');
        b.resourceId = std::stoi(t);
        std::getline(in, b.issueDate, '|');
        std::getline(in, b.dueDate, '|');
        std::getline(in, b.returnDate, '|');
        std::getline(in, t);
        b.fineAmount = std::stod(t);
        return in;
    }
};