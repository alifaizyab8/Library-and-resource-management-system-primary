#pragma once
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include "../domain/User.h"
#include "../domain/Resource.h"
#include "../domain/Transaction.h"
#include "../domain/Fine.h"
#include "../domain/BorrowingHistory.h"
#include "../domain/FundRequest.h"

// Forward declarations
class UserStore;
class ResourceStore;
class TransactionStore;
class FineStore;
class BorrowingHistoryStore;
class FundRequestStore;

class UserService
{
private:
    UserStore &userStore;
    ResourceStore &resourceStore;
    TransactionStore &transactionStore;
    FineStore &fineStore;
    BorrowingHistoryStore &historyStore;
    FundRequestStore &fundStore;

public:
    UserService(UserStore &uStore, ResourceStore &rStore, TransactionStore &tStore,
                FineStore &fStore, BorrowingHistoryStore &hStore, FundRequestStore &frStore);

    // resource related functions
    std::vector<Resource> searchResources(const std::string &query);
    std::vector<Resource> getAvailableResources();

    // borrowal etc
    bool borrowResource(int userId, int resourceId);
    bool returnResource(int userId, int transactionId, const std::string &returnDate);
    std::vector<Transaction> getActiveBorrows(int userId);
    std::vector<BorrowingHistory> getBorrowingHistory(int userId);

    // fine related
    std::vector<Fine> getPendingFines(int userId);
    bool payFine(int userId, int fineId, const std::string &payDate);
    bool submitFundRequest(int userId, double amount, const std::string &reqDate);
    std::vector<FundRequest> getFundRequests(int userId);

    // account
    std::unique_ptr<User> getProfile(int userId);
    bool updateContactInfo(int userId, const std::string &phone, const std::string &address);
    bool requestAccountDeletion(int userId);
};