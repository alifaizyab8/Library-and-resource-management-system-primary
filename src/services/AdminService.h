#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../domain/User.h"
#include "../domain/Fine.h"
#include "../domain/Resource.h"
#include "../domain/Category.h"
#include "../domain/FundRequest.h"
#include "../domain/Transaction.h"
#include "../domain/Administrator.h"
#include "../domain/MembershipType.h"
#include "../domain/BorrowingHistory.h"

class UserStore;
class FineStore;
class ResourceStore;
class CategoryStore;
class FundRequestStore;
class TransactionStore;
class ReservationStore;
class MembershipTypeStore;
class AdministratorStore;
class BorrowingHistoryStore;

class AdminService
{
private:
    UserStore &userStore;
    FineStore &fineStore;
    ResourceStore &resourceStore;
    CategoryStore &categoryStore;
    FundRequestStore &fundRequestStore;
    TransactionStore &transactionStore;
    MembershipTypeStore &membershipTypeStore;
    BorrowingHistoryStore &borrowingHistoryStore;
    AdministratorStore &administratorStore;

public:
    /* **************************************************************************
              --------- CONSTRUCTOR ---------
       ************************************************************************** */
    AdminService(UserStore &userRepo, FineStore &fineRepo, ResourceStore &resourceRepo,
                 CategoryStore &categoryRepo, FundRequestStore &fundRequestRepo, TransactionStore &transactionRepo,
                 MembershipTypeStore &membershipTypeRepo,
                 BorrowingHistoryStore &borrowingHistoryRepo, AdministratorStore &administratorRepo);

    /* **************************************************************************
              --------- CATALOG & CATEGORY MANAGEMENT ---------
       ************************************************************************** */
    bool deleteResource(int resourceId);
    bool addResource(Resource &resource);
    bool editResource(Resource &updatedResource);
    std::unique_ptr<Resource> getResourceById(int resourceId);
    std::vector<Resource> viewAllResources();

    bool deleteCategory(int categoryId);
    bool addCategory(Category &category);
    bool editCategory(Category &updatedCategory);
    std::vector<Category> viewAllCategories();
    std::unique_ptr<Category> getCategoryById(int categoryId);

    /* **************************************************************************
              --------- REQUEST APPROVALS ---------
       ************************************************************************** */
    std::vector<Transaction> viewPendingBorrowRequests();
    std::vector<FundRequest> viewPendingFundRequests();

    /* **************************************************************************
              --------- USER MANAGEMENT ---------
       ************************************************************************** */
    bool addUser(User &user);
    bool editUser(User &updatedData);
    bool deleteUserAccount(int userId);
    bool suspendUserAccount(int userId);
    bool reactivateUserAccount(int userId);
    std::vector<User> viewAllUsers();
    std::vector<User> viewDeletionRequests();
    std::unique_ptr<User> getUserById(int userId);

    /* **************************************************************************
              --------- FINE MANAGEMENT ---------
       ************************************************************************** */
    std::vector<Fine> viewAllFines();
    std::vector<Fine> viewFinesByUser(int userId);
    bool imposeFine(Fine &fine);
    bool markFineAsPaid(int fineId);
    bool deleteFine(int fineId);
    bool updateFine(Fine &fine);
    void updateDailyFines(const std::string &simulatedToday);
    std::unique_ptr<Fine> getFineById(int fineId);

    /* **************************************************************************
              --------- REPORTING ---------
       ************************************************************************** */
    bool generateUserHistoryReport(const std::string &filename);
    bool generateIssuedAndOverdueReport(const std::string &filename);

    /* **************************************************************************
              --------- PROCESS & WORKFLOW ---------
       ************************************************************************** */
    bool processFundRequest(int fundRequestId, bool approve, std::string &dateToday);
    bool processReturn(int transactionId, std::string &dateToday);
    bool processBorrowRequest(int transactionId, bool approve, std::string &dateToday);
    bool processAccountDeletionRequest(int userId, bool approve);

    /* **************************************************************************
              --------- Membership Management ---------
       ************************************************************************** */
    std::vector<MembershipType> viewAllMembershipTypes();
    bool addMembershipType(MembershipType &type);
    bool editMembershipType(MembershipType &type);
    bool deleteMembershipType(int typeId);
    std::unique_ptr<MembershipType> getMembershipTypeById(int typeId);

    /* **************************************************************************
              --------- Transaction Management ---------
       ************************************************************************** */
    std::vector<Transaction> viewTransactionsByUser(int userId);
    std::vector<Transaction> viewAllTransactions();

    /* **************************************************************************
              --------- Admin Management ---------
       ************************************************************************** */
    std::vector<Administrator> viewAllAdministrators();
    bool addAdministrator(Administrator &admin);
    bool deleteAdministrator(int adminId);
};