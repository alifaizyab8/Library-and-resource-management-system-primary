#pragma once
#include <string>
#include <iostream>
#include <fstream>

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
        : fineId(id), transactionId(tId), userId(uId), daysOverdue(days), fineAmount(amount),
          fineDate(fDate), isPaid(paid), paymentDate(pDate) {}

    int getFineId() const { return fineId; }
    int getTransactionId() const { return transactionId; }
    int getUserId() const { return userId; }
    int getDaysOverdue() const { return daysOverdue; }
    double getFineAmount() const { return fineAmount; }
    std::string getFineDate() const { return fineDate; }
    bool getIsPaid() const { return isPaid; }
    std::string getPaymentDate() const { return paymentDate; }

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

    friend std::ofstream &writeToFile(std::ofstream &out, const Fine &f)
    {
        out << f.fineId << '|' << f.transactionId << '|' << f.userId << '|'
            << f.daysOverdue << '|' << f.fineAmount << '|' << f.fineDate << '|'
            << f.isPaid << '|' << f.paymentDate;
        return out;
    }

    friend std::ifstream &readFromFile(std::ifstream &in, Fine &f)
    {
        std::string t;
        if (!std::getline(in, t, '|'))
            return in;
        f.fineId = std::stoi(t);
        std::getline(in, t, '|');
        f.transactionId = std::stoi(t);
        std::getline(in, t, '|');
        f.userId = std::stoi(t);
        std::getline(in, t, '|');
        f.daysOverdue = std::stoi(t);
        std::getline(in, t, '|');
        f.fineAmount = std::stod(t);
        std::getline(in, f.fineDate, '|');
        std::getline(in, t, '|');
        f.isPaid = std::stoi(t);
        std::getline(in, f.paymentDate);
        return in;
    }
};