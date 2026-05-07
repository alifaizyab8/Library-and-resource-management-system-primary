#include "FinancialStore.h"

// Transaction store
TransactionStore::TransactionStore(const std::string &filename) : BaseRepository<Transaction>(filename) {}

// Dummy
bool TransactionStore::beginTransaction() { return true; }
bool TransactionStore::commitTransaction() { return true; }
bool TransactionStore::rollbackTransaction() { return true; }

bool TransactionStore::save(Transaction &transaction)
{
    if (transaction.getTransactionId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getTransactionId() + 1;
        transaction.setTransactionId(newId);
        arr.push_back(transaction);
    }
    else
    {
        bool found = false;
        for (auto &t : arr)
        {
            if (t.getTransactionId() == transaction.getTransactionId())
            {
                t = transaction;
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    saveToFile();
    return true;
}

bool TransactionStore::deleteTransaction(int transactionId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getTransactionId() == transactionId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<Transaction> TransactionStore::getById(int transactionId)
{
    for (const auto &t : arr)
    {
        if (t.getTransactionId() == transactionId)
            return std::make_unique<Transaction>(t);
    }
    return nullptr;
}

std::vector<Transaction> TransactionStore::getByUserId(int userId)
{
    std::vector<Transaction> results;
    for (const auto &t : arr)
    {
        if (t.getUserId() == userId)
            results.push_back(t);
    }
    return results;
}

std::vector<Transaction> TransactionStore::getAllTransactions()
{
    return arr;
}

std::vector<Transaction> TransactionStore::getActiveIssues()
{
    std::vector<Transaction> results;
    for (const auto &t : arr)
    {
        if (!t.getIsReturned() && t.getTransactionStatus() == "ISSUED")
        {
            results.push_back(t);
        }
    }
    return results;
}

std::vector<Transaction> TransactionStore::getbyStatus(const std::string &status)
{
    std::vector<Transaction> results;
    for (const auto &t : arr)
    {
        if (t.getTransactionStatus() == status)
            results.push_back(t);
    }
    return results;
}

/* =========================================================
                     FINE STORE LOGIC
   ========================================================= */

FineStore::FineStore(const std::string &filename) : BaseRepository<Fine>(filename) {}

bool FineStore::save(Fine &fine)
{
    if (fine.getFineId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getFineId() + 1;
        fine.setFineId(newId);
        arr.push_back(fine);
    }
    else
    {
        bool found = false;
        for (auto &f : arr)
        {
            if (f.getFineId() == fine.getFineId())
            {
                f = fine;
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    saveToFile();
    return true;
}

bool FineStore::deleteFine(int fineId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getFineId() == fineId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<Fine> FineStore::getById(int fineId)
{
    for (const auto &f : arr)
    {
        if (f.getFineId() == fineId)
            return std::make_unique<Fine>(f);
    }
    return nullptr;
}

std::vector<Fine> FineStore::getByUserId(int userId)
{
    std::vector<Fine> results;
    for (const auto &f : arr)
    {
        if (f.getUserId() == userId)
            results.push_back(f);
    }
    return results;
}

std::vector<Fine> FineStore::getAllFines()
{
    return arr;
}

/* =========================================================
                 FUND REQUEST STORE LOGIC
   ========================================================= */

FundRequestStore::FundRequestStore(const std::string &filename) : BaseRepository<FundRequest>(filename) {}

bool FundRequestStore::save(FundRequest &request)
{
    if (request.getRequestId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getRequestId() + 1;
        request.setRequestId(newId);
        arr.push_back(request);
    }
    else
    {
        bool found = false;
        for (auto &r : arr)
        {
            if (r.getRequestId() == request.getRequestId())
            {
                r = request;
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    saveToFile();
    return true;
}

bool FundRequestStore::deleteFundRequest(int requestId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getRequestId() == requestId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<FundRequest> FundRequestStore::getById(int requestId)
{
    for (const auto &r : arr)
    {
        if (r.getRequestId() == requestId)
            return std::make_unique<FundRequest>(r);
    }
    return nullptr;
}

std::vector<FundRequest> FundRequestStore::getByUserId(int userId)
{
    std::vector<FundRequest> results;
    for (const auto &r : arr)
    {
        if (r.getUserId() == userId)
            results.push_back(r);
    }
    return results;
}

std::vector<FundRequest> FundRequestStore::getAllFundRequests()
{
    return arr;
}