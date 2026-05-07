#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class BorrowingHistory
{
private:
    int historyId;
    int userId;
    int resourceId;
    string issueDate;
    string dueDate;
    string returnDate;
    double fineAmount;

public:
    // Constructors
    BorrowingHistory()
        : historyId(0), userId(0), resourceId(0), issueDate(""),
          dueDate(""), returnDate(""), fineAmount(0.0) {}

    BorrowingHistory(int userId, int resourceId, string issueDate,
                     string dueDate, string returnDate, double fine)
        : historyId(0), userId(userId), resourceId(resourceId),
          issueDate(issueDate), dueDate(dueDate), returnDate(returnDate),
          fineAmount(fine) {}

    // Getters
    int getId() const { return historyId; }
    int getUserId() const { return userId; }
    int getResourceId() const { return resourceId; }
    string getIssueDate() const { return issueDate; }
    string getDueDate() const { return dueDate; }
    string getReturnDate() const { return returnDate; }
    double getFineAmount() const { return fineAmount; }

    // Setter
    void setId(int id) { historyId = id; }
    void setReturnDate(const string &date) { returnDate = date; }
    void setFineAmount(double fine) { fineAmount = fine; }
    friend ofstream &writeToFile(ofstream &out, const BorrowingHistory &b)
    {
        out << b.historyId << '|' << b.userId << '|' << b.resourceId << '|'
            << b.issueDate << '|' << b.dueDate << '|' << b.returnDate << '|' << b.fineAmount;
        return out;
    }
    friend ifstream &readFromFile(ifstream &in, BorrowingHistory &b)
    {
        string t;
        if (!getline(in, t, '|'))
            return in;
        b.historyId = stoi(t);
        getline(in, t, '|');
        b.userId = stoi(t);
        getline(in, t, '|');
        b.resourceId = stoi(t);
        getline(in, b.issueDate, '|');
        getline(in, b.dueDate, '|');
        getline(in, b.returnDate, '|');
        getline(in, t);
        b.fineAmount = stod(t);
        return in;
    }
};
