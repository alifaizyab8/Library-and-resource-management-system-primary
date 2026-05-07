#include "UserService.h"
#include "../dataStore/AssetStore.h"    
#include "../dataStore/FinancialStore.h"
#include "../dataStore/AdminUserStore.h"

// Constructor 
UserService::UserService(UserStore &uStore, ResourceStore &rStore, TransactionStore &tStore,
                         FineStore &fStore, BorrowingHistoryStore &hStore, FundRequestStore &frStore)
    : userStore(uStore), resourceStore(rStore), transactionStore(tStore),
      fineStore(fStore), historyStore(hStore), fundStore(frStore) {}

// resource related

std::vector<Resource> UserService::searchResources(const std::string &keyword)
{
    std::vector<Resource> results;
    for (const auto &res : resourceStore.getAll())
    {
        // Simple search checking if keyword exists in title or author
        if (res.getIsActive() &&
            (res.getTitle().find(keyword) != std::string::npos || res.getAuthor().find(keyword) != std::string::npos))
        {
            results.push_back(res);
        }
    }
    return results;
}

std::vector<Resource> UserService::getAvailableResources()
{
    std::vector<Resource> available;
    for (const auto &res : resourceStore.getAll())
    {
        if (res.getIsActive() && res.getAvailableCopies() > 0)
            available.push_back(res);
    }
    return available;
}

// borrowal etc

bool UserService::borrowResource(int userId, int resourceId)
{
    auto res = resourceStore.getById(resourceId);
    if (!res || res->getAvailableCopies() <= 0)
        throw std::logic_error("Resource unavailable.");

    // Check for unpaid fines
    for (const auto &fine : fineStore.getByUserId(userId))
    {
        if (!fine.getIsPaid())
            throw std::logic_error("Clear unpaid fines before borrowing.");
    }

    Transaction txn(0, userId, resourceId, "", "", "", 0.0, false, false, 0, "PENDING");
    return transactionStore.save(txn);
}

bool UserService::returnResource(int userId, int transactionId, const std::string &returnDate)
{
    auto txn = transactionStore.getById(transactionId);
    if (!txn || txn->getUserId() != userId || txn->getIsReturned())
        return false;

    txn->setIsReturned(true);
    txn->setReturnDate(returnDate);
    txn->setTransactionStatus("RETURNED");

    // Increase available copies
    auto res = resourceStore.getById(txn->getResourceId());
    if (res)
    {
        res->setAvailableCopies(res->getAvailableCopies() + 1);
        resourceStore.save(*res);
    }

    return transactionStore.save(*txn);
}
std::vector<Transaction> UserService::getActiveBorrows(int userId)
{
    std::vector<Transaction> active;
    for (const auto &txn : transactionStore.getByUserId(userId))
    {
        if (!txn.getIsReturned() && txn.getTransactionStatus() == "ISSUED")
            active.push_back(txn);
    }
    return active;
}

std::vector<BorrowingHistory> UserService::getBorrowingHistory(int userId)
{
    return historyStore.getByUserId(userId);
}

// fine related

std::vector<Fine> UserService::getPendingFines(int userId)
{
    std::vector<Fine> pending;
    for (const auto &fine : fineStore.getByUserId(userId))
    {
        if (!fine.getIsPaid())
            pending.push_back(fine);
    }
    return pending;
}

bool UserService::payFine(int userId, int fineId, const std::string &payDate)
{
    auto fine = fineStore.getById(fineId);
    if (!fine || fine->getUserId() != userId || fine->getIsPaid())
        return false;

    auto user = userStore.getById(userId);
    if (!user || user->getBalance() < fine->getFineAmount())
    {
        throw std::logic_error("Insufficient wallet balance.");
    }

    // Process payment
    user->setBalance(user->getBalance() - fine->getFineAmount());
    fine->setIsPaid(true);
    fine->setPaymentDate(payDate);

    return userStore.save(*user) && fineStore.save(*fine);
}

bool UserService::submitFundRequest(int userId, double amount, const std::string &reqDate)
{
    if (amount <= 0)
        throw std::invalid_argument("Amount must be positive.");
    FundRequest req(0, userId, amount, reqDate, "PENDING", 0, "", "");
    return fundStore.save(req);
}

std::vector<FundRequest> UserService::getFundRequests(int userId)
{
    return fundStore.getByUserId(userId);
}

//account

std::unique_ptr<User> UserService::getProfile(int userId)
{
    return userStore.getById(userId);
}

bool UserService::updateContactInfo(int userId, const std::string &phone, const std::string &address)
{
    auto user = userStore.getById(userId);
    if (!user)
        throw std::invalid_argument("User not found.");

    user->setPhone(phone);
    user->setAddress(address);
    return userStore.save(*user);
}

bool UserService::requestAccountDeletion(int userId)
{
    auto user = userStore.getById(userId);
    if (!user)
        return false;

    if (!getPendingFines(userId).empty() || !getActiveBorrows(userId).empty())
    {
        throw std::logic_error("Clear fines and return books before deleting account.");
    }

    user->setDeletionRequested(true);
    return userStore.save(*user);
}