#pragma once
#include <string>
#include <iostream>
#include<fstream>
using namespace std;

class Fine
{
private:
    int fineId;
    int transactionId;
    int userId;
    int daysOverdue;
    double fineAmount;
    std::string fineDate;
    bool isPaid;
    std::string paymentDate;

public:
    Fine() : fineId(0), transactionId(0), userId(0), daysOverdue(0), fineAmount(0.0), isPaid(false) {}

    Fine(int id, int tId, int uId, int days, double amount, const std::string &fDate, bool paid, const std::string &pDate)
        : fineId(id), transactionId(tId), userId(uId), daysOverdue(days), fineAmount(amount), fineDate(fDate), isPaid(paid), paymentDate(pDate) {}

    // Getters
    int getFineId() const { return fineId; }
    int getTransactionId() const { return transactionId; }
    int getUserId() const { return userId; }
    int getDaysOverdue() const { return daysOverdue; }
    double getFineAmount() const { return fineAmount; }
    std::string getFineDate() const { return fineDate; }
    bool getIsPaid() const { return isPaid; }
    std::string getPaymentDate() const { return paymentDate; }

    // Setters
    void setFineId(int id) { fineId = id; }
    void setTransactionId(int tId) { transactionId = tId; }
    void setUserId(int uId) { userId = uId; }
    void setDaysOverdue(int days) { daysOverdue = days; }
    void setFineAmount(double amount) { fineAmount = amount; }
    void setFineDate(const std::string &fDate) { fineDate = fDate; }
    void setIsPaid(bool paid) { isPaid = paid; }
    void setPaymentDate(const std::string &pDate) { paymentDate = pDate; }

    friend std::ostream &operator<<(std::ostream &os, const Fine &f)
    {
        os << "Fine ID: " << f.getFineId()
           << " | User ID: " << f.getUserId()
           << " | Txn ID: " << f.getTransactionId()
           << " | Amount: $" << f.getFineAmount()
           << " | Status: " << (f.getIsPaid() ? "PAID" : "UNPAID");
        return os;
    }
    friend ofstream &writeToFile(ofstream &out, const Fine &f)
    {
        out << f.fineId << '|' << f.transactionId << '|' << f.userId << '|'
            << f.daysOverdue << '|' << f.fineAmount << '|' << f.fineDate << '|'
            << f.isPaid << '|' << f.paymentDate;
        return out;
    }
    friend ifstream &readFromFile(ifstream &in, Fine &f)
    {
        string t;
        if (!getline(in, t, '|'))
            return in;
        f.fineId = stoi(t);
        getline(in, t, '|');
        f.transactionId = stoi(t);
        getline(in, t, '|');
        f.userId = stoi(t);
        getline(in, t, '|');
        f.daysOverdue = stoi(t);
        getline(in, t, '|');
        f.fineAmount = stod(t);
        getline(in, f.fineDate, '|');
        getline(in, t, '|');
        f.isPaid = stoi(t);
        getline(in, f.paymentDate);
        return in;
    }
};