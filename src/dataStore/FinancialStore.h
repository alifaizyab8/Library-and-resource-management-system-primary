#pragma once
#include "BaseRepository.h"
#include "domain/Transaction.h"
#include "domain/Fine.h"
#include "domain/FundRequest.h"

// transaction store
class TransactionStore : public BaseRepository<Transaction>
{
public:
    explicit TransactionStore(const std::string &filename);



    bool save(Transaction &transaction);
    bool deleteTransaction(int transactionId);

    std::unique_ptr<Transaction> getById(int transactionId);
    std::vector<Transaction> getByUserId(int userId);
    std::vector<Transaction> getAllTransactions();

    std::vector<Transaction> getActiveIssues();
    std::vector<Transaction> getbyStatus(const std::string &status);
};

// fine store
class FineStore : public BaseRepository<Fine>
{
public:
    explicit FineStore(const std::string &filename);

    bool save(Fine &fine);
    bool deleteFine(int fineId);

    std::unique_ptr<Fine> getById(int fineId);
    std::vector<Fine> getByUserId(int userId);
    std::vector<Fine> getAllFines();
};

// fundRequest store
class FundRequestStore : public BaseRepository<FundRequest>
{
public:
    explicit FundRequestStore(const std::string &filename);

    bool save(FundRequest &request);
    bool deleteFundRequest(int requestId);

    std::unique_ptr<FundRequest> getById(int requestId);
    std::vector<FundRequest> getByUserId(int userId);
    std::vector<FundRequest> getAllFundRequests();
};