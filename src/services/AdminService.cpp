#include "AdminService.h"
#include "../PDFGenerator/PdfGenerator.h"
#include "../Utility/date.h"
#include <sstream>

#include "../dataStore/AdminUserStore.h"
#include "../dataStore/AssetStore.h"
#include "../dataStore/FinancialStore.h"

/* *************************************************************************
                 ---------- CONSTRUCTORS & DESTRUCTORS ----------
   ************************************************************************* */

AdminService::AdminService(UserStore &userRepo, FineStore &fineRepo, ResourceStore &resourceRepo,
                           CategoryStore &categoryRepo, FundRequestStore &fundRequestRepo, TransactionStore &transactionRepo,
                            MembershipTypeStore &membershipTypeRepo,
                           BorrowingHistoryStore &borrowingHistoryRepo, AdministratorStore &administratorRepo)
    : userStore(userRepo), fineStore(fineRepo), resourceStore(resourceRepo), categoryStore(categoryRepo),
      fundRequestStore(fundRequestRepo), transactionStore(transactionRepo),
      membershipTypeStore(membershipTypeRepo), borrowingHistoryStore(borrowingHistoryRepo), administratorStore(administratorRepo) {}

/* *************************************************************************
                 ---------- RESOURCE MANAGEMENT ----------
   ************************************************************************* */

bool AdminService::addResource(Resource &resource)
{
    return resourceStore.save(resource);
}

bool AdminService::editResource(Resource &updatedResource)
{
    return resourceStore.save(updatedResource);
}

bool AdminService::deleteResource(int resourceId)
{
    return resourceStore.deleteResource(resourceId);
}

std::unique_ptr<Resource> AdminService::getResourceById(int resourceId)
{
    return resourceStore.getById(resourceId);
}

std::vector<Resource> AdminService::viewAllResources()
{
    return resourceStore.getAll();
}

std::vector<Category> AdminService::viewAllCategories()
{
    return categoryStore.getAll();
}

/* *************************************************************************
                 ---------- CATEGORY MANAGEMENT ----------
   ************************************************************************* */

std::unique_ptr<Category> AdminService::getCategoryById(int categoryId)
{
    return categoryStore.getById(categoryId);
}

bool AdminService::addCategory(Category &category)
{
    return categoryStore.save(category);
}

bool AdminService::editCategory(Category &updatedCategory)
{
    return categoryStore.save(updatedCategory);
}

bool AdminService::deleteCategory(int categoryId)
{
    return categoryStore.deleteCategory(categoryId);
}

/* *************************************************************************
                 ---------- USER MANAGEMENT ----------
   ************************************************************************* */

bool AdminService::addUser(User &user)
{
    return userStore.save(user);
}

bool AdminService::editUser(User &updatedData)
{
    return userStore.save(updatedData);
}

bool AdminService::suspendUserAccount(int userId)
{
    std::unique_ptr<User> user = userStore.getById(userId);

    if (!user)
    {
        return false;
    }
    user->setIsActive(false);
    return userStore.save(*user);
}

bool AdminService::reactivateUserAccount(int userId)
{
    std::unique_ptr<User> user = userStore.getById(userId);
    if (!user)
        return false;

    user->setIsActive(true);
    return userStore.save(*user);
}

std::vector<User> AdminService::viewAllUsers()
{
    return userStore.getAllUsers();
}

std::vector<User> AdminService::viewDeletionRequests()
{
    return userStore.getPendingDeletionRequests();
}

bool AdminService::processAccountDeletionRequest(int userId, bool approve)
{
    std::unique_ptr<User> user = userStore.getById(userId);

    if (!user)
    {
        return false;
    }

    if (approve)
    {
        return deleteUserAccount(userId);
    }
    else
    {
        user->setDeletionRequested(false);
        return userStore.save(*user);
    }
}

bool AdminService::deleteUserAccount(int userId)
{
    return userStore.deleteUser(userId);
}

std::unique_ptr<User> AdminService::getUserById(int userId)
{
    return userStore.getById(userId);
}

/* *************************************************************************
                 ---------- FINE MANAGEMENT ----------
   ************************************************************************* */

bool AdminService::updateFine(Fine &fine)
{
    return fineStore.save(fine);
}

bool AdminService::deleteFine(int fineId)
{
    return fineStore.deleteFine(fineId);
}

std::vector<Fine> AdminService::viewAllFines()
{
    return fineStore.getAllFines();
}

std::vector<Fine> AdminService::viewFinesByUser(int userId)
{
    return fineStore.getByUserId(userId);
}

bool AdminService::imposeFine(Fine &fine)
{
    return fineStore.save(fine);
}

bool AdminService::markFineAsPaid(int fineId)
{
    std::unique_ptr<Fine> fine = fineStore.getById(fineId);

    if (!fine)
    {
        return false;
    }

    fine->setIsPaid(true);
    return fineStore.save(*fine);
}
std::unique_ptr<Fine> AdminService::getFineById(int fineId)
{
    return fineStore.getById(fineId);
}

void AdminService::updateDailyFines(const std::string &dateToday)
{
    std::vector<Transaction> activeTransactions = transactionStore.getActiveIssues();

    for (Transaction &txn : activeTransactions)
    {
        if (dateToday > txn.getDueDate())
        {
            txn.setIsOverdue(true);

            int daysLate = calculateDaysOverdue(txn.getDueDate(), dateToday);

            if (daysLate > 0)
            {
                double currentFineAmount = daysLate * 5.0;

                bool fineExists = false;
                std::vector<Fine> userFines = fineStore.getByUserId(txn.getUserId());

                for (Fine &existingFine : userFines)
                {
                    if (existingFine.getTransactionId() == txn.getTransactionId())
                    {
                        existingFine.setDaysOverdue(daysLate);
                        existingFine.setFineAmount(currentFineAmount);
                        existingFine.setFineDate(dateToday);
                        fineStore.save(existingFine);
                        fineExists = true;
                        break;
                    }
                }

                if (!fineExists)
                {
                    Fine newFine;
                    newFine.setTransactionId(txn.getTransactionId());
                    newFine.setUserId(txn.getUserId());
                    newFine.setDaysOverdue(daysLate);
                    newFine.setFineAmount(currentFineAmount);
                    newFine.setFineDate(dateToday);
                    newFine.setIsPaid(false);

                    fineStore.save(newFine);
                }

                txn.setFineAmount(currentFineAmount);
                transactionStore.save(txn);
            }
        }
    }
}

/* *************************************************************************
                 ---------- REPORTING ----------
   ************************************************************************* */

bool AdminService::generateUserHistoryReport(const std::string &filename)
{
    std::stringstream reportContent;

    reportContent << endl;
    reportContent << "Complete Customer Borrowing History\n";
    reportContent << "===================================\n";

    std::vector<User> allUsers = userStore.getAllUsers();

    if (allUsers.empty())
    {
        reportContent << "No customers found in the system.\n";
    }

    for (const User &user : allUsers)
    {

        reportContent << "-------------------------------------------------\n";
        reportContent << "Customer ID: " << user.getUserId() << "\n";
        reportContent << "Name: " << user.getFirstName() << " " << user.getLastName() << "\n";
        reportContent << "Email: " << user.getEmail() << "\n";
        reportContent << "-------------------------------------------------\n";

        std::vector<BorrowingHistory> history = borrowingHistoryStore.getByUserId(user.getUserId());

        if (history.empty())
        {

            reportContent << "[No Borrowing History Found]\n\n";
            continue;
        }

        for (const BorrowingHistory &record : history)
        {
            std::string bookTitle = "Unknown Resource";

            std::unique_ptr<Resource> resource = resourceStore.getById(record.getResourceId());
            if (resource)
            {
                bookTitle = resource->getTitle();
            }
            reportContent << "   * Borrowed: '" << bookTitle << "' (Resource ID: " << record.getResourceId() << ")\n"
                          << "     Issue Date: " << (record.getIssueDate().empty() ? "Pending" : record.getIssueDate()) << "\n"
                          << "     Due Date:   " << (record.getDueDate().empty() ? "Pending" : record.getDueDate()) << "\n"
                          << "     Returned:   " << (record.getReturnDate().empty() ? "Not Returned Yet" : record.getReturnDate()) << "\n"
                          << "     Fine Paid:  $" << record.getFineAmount() << "\n\n";
        }
    }
    std::string finalString = reportContent.str();

    makePdf(filename, "Customer Borrowing History Report", finalString);

    return true;
}

bool AdminService::generateIssuedAndOverdueReport(const std::string &filename)
{
    std::stringstream reportContent;

    reportContent << endl;
    reportContent << "Issued and Overdue Resources Report\n";
    reportContent << "===================================\n";

    std::vector<Transaction> activeTransactions = transactionStore.getActiveIssues();
    if (activeTransactions.empty())
    {
        reportContent << "No active issued resources found.\n";
        makePdf(filename, "Issued and Overdue Resources Report", reportContent.str());
        return true;
    }

    std::stringstream issuedSection;
    std::stringstream overdueSection;

    int overdueCount = 0;
    int issuedCount = 0;

    for (const Transaction &txn : activeTransactions)
    {
        std::unique_ptr<Resource> resource = resourceStore.getById(txn.getResourceId());
        std::unique_ptr<User> user = userStore.getById(txn.getUserId());

        std::string resourceTitle = resource ? resource->getTitle() : "Unknown Resource";
        std::string userName = (user) ? (user->getFirstName() + " " + user->getLastName()) : "Unknown User";

        std::string entry = "   * '" + resourceTitle + "' (Resource ID: " + std::to_string(txn.getResourceId()) + ")\n" + "     Borrowed by: " + userName + " (User ID: " + std::to_string(txn.getUserId()) + ")\n" + "     Issue Date: " + (txn.getIssueDate().empty() ? "Pending" : txn.getIssueDate()) + "\n" + "     Due Date:   " + (txn.getDueDate().empty() ? "Pending" : txn.getDueDate()) + "\n\n";

        if (txn.getIsOverdue())
        {
            overdueSection << entry;
            overdueCount++;
        }
        else
        {
            issuedSection << entry;
            issuedCount++;
        }
    }

    reportContent << "--- OVERDUE RESOURCES (" << overdueCount << ") ---\n";
    reportContent << (overdueCount > 0 ? overdueSection.str() : "   [None]\n") << "\n";

    reportContent << "--- CURRENTLY ISSUED IN GOOD STANDING (" << issuedCount << ") ---\n";
    reportContent << (issuedCount > 0 ? issuedSection.str() : "   [None]\n") << "\n";

    makePdf(filename, "Issued and Overdue Report", reportContent.str());

    return true;
}

/* *************************************************************************
                 ---------- TRANSACTION PROCESSING ----------
   ************************************************************************* */

std::vector<Transaction> AdminService::viewPendingBorrowRequests()
{
    return transactionStore.getbyStatus("PENDING");
}

bool AdminService::processBorrowRequest(int transactionId, bool approve, std::string &dateToday)
{
    std::unique_ptr<Transaction> transaction = transactionStore.getById(transactionId);
    if (!transaction)
        return false;

    transactionStore.beginTransaction();

    if (approve)
    {
        transaction->setTransactionStatus("ISSUED");
        transaction->setIssueDate(dateToday);
        transaction->setDueDate(getDueDate(14, dateToday));

        std::unique_ptr<Resource> resource = resourceStore.getById(transaction->getResourceId());
        if (resource && resource->getIsActive() && resource->getAvailableCopies() > 0)
        {
            resource->setAvailableCopies(resource->getAvailableCopies() - 1);

            if (!resourceStore.save(*resource))
            {
                transactionStore.rollbackTransaction();
                return false;
            }
        }
        else
        {
            transactionStore.rollbackTransaction();
            return false;
        }

        BorrowingHistory historyRecord(
            transaction->getUserId(), transaction->getResourceId(),
            transaction->getIssueDate(), transaction->getDueDate(), "", 0.0);

        if (!borrowingHistoryStore.save(historyRecord))
        {
            transactionStore.rollbackTransaction();
            return false;
        }
    }
    else
    {
        transaction->setTransactionStatus("REJECTED");
    }

    if (transactionStore.save(*transaction))
    {
        transactionStore.commitTransaction();
        return true;
    }
    else
    {
        transactionStore.rollbackTransaction();
        return false;
    }
}

bool AdminService::processReturn(int transactionId, std::string &dateToday)
{
    std::unique_ptr<Transaction> txn = transactionStore.getById(transactionId);

    if (!txn || txn->getTransactionStatus() != "ISSUED")
    {
        return false;
    }

    transactionStore.beginTransaction();

    std::string today = dateToday;

    txn->setTransactionStatus("RETURNED");
    txn->setIsReturned(true);
    txn->setReturnDate(today);

    std::unique_ptr<Resource> resource = resourceStore.getById(txn->getResourceId());
    if (resource)
    {
        resource->setAvailableCopies(resource->getAvailableCopies() + 1);

        if (!resourceStore.save(*resource))
        {
            transactionStore.rollbackTransaction();
            return false;
        }
    }

    std::vector<BorrowingHistory> userHistory = borrowingHistoryStore.getByUserId(txn->getUserId());
    for (BorrowingHistory &history : userHistory)
    {
        if (history.getResourceId() == txn->getResourceId() && history.getReturnDate().empty())
        {
            history.setReturnDate(today);
            history.setFineAmount(txn->getFineAmount());

            if (!borrowingHistoryStore.save(history))
            {
                transactionStore.rollbackTransaction();
                return false;
            }
            break;
        }
    }

    if (transactionStore.save(*txn))
    {
        transactionStore.commitTransaction();
        return true;
    }
    else
    {
        transactionStore.rollbackTransaction();
        return false;
    }
}

std::vector<Transaction> AdminService::viewTransactionsByUser(int userId)
{
    return transactionStore.getByUserId(userId);
}

std::vector<Transaction> AdminService::viewAllTransactions()
{
    return transactionStore.getAllTransactions();
}

/* *************************************************************************
                 ---------- FUND REQUEST PROCESSING ----------
   ************************************************************************* */

std::vector<FundRequest> AdminService::viewPendingFundRequests()
{
    return fundRequestStore.getAllFundRequests();
}

bool AdminService::processFundRequest(int fundRequestId, bool approve, std::string &dateToday)
{
    std::unique_ptr<FundRequest> request = fundRequestStore.getById(fundRequestId);

    if (!request || request->getStatus() != "PENDING")
        return false;

    request->setApprovalDate(dateToday);

    transactionStore.beginTransaction();

    if (approve)
    {
        std::unique_ptr<User> user = userStore.getById(request->getUserId());
        if (!user)
        {
            transactionStore.rollbackTransaction();
            return false;
        }

        double newBalance = user->getBalance() + request->getRequestedAmount();
        user->setBalance(newBalance);
        request->setStatus("APPROVED");
        request->setAdminNotes("Approved");

        if (newBalance >= 50.0 && user->getMembershipTypeId() == 1)
        {
            user->setMembershipTypeId(2);
        }

        bool userSaved = userStore.save(*user);
        bool requestSaved = fundRequestStore.save(*request);

        if (userSaved && requestSaved)
        {
            transactionStore.commitTransaction();
            return true;
        }
        else
        {
            transactionStore.rollbackTransaction();
            return false;
        }
    }
    else
    {
        request->setStatus("REJECTED");
        request->setAdminNotes("Rejected");

        if (fundRequestStore.save(*request))
        {
            transactionStore.commitTransaction();
            return true;
        }
        else
        {
            transactionStore.rollbackTransaction();
            return false;
        }
    }
}

/* *************************************************************************
                 ---------- MEMBERSHIP TYPE MANAGEMENT ----------
   ************************************************************************* */

std::vector<MembershipType> AdminService::viewAllMembershipTypes()
{
    return membershipTypeStore.getAllMembershipTypes();
}

bool AdminService::addMembershipType(MembershipType &type)
{
    return membershipTypeStore.save(type);
}

bool AdminService::editMembershipType(MembershipType &updatedType)
{
    return membershipTypeStore.save(updatedType);
}

bool AdminService::deleteMembershipType(int typeId)
{
    return membershipTypeStore.deleteMembershipType(typeId);
}

std::unique_ptr<MembershipType> AdminService::getMembershipTypeById(int typeId)
{
    return membershipTypeStore.getById(typeId);
}

/* *************************************************************************
                 ---------- ADMINISTRATOR MANAGEMENT ----------
   ************************************************************************* */

std::vector<Administrator> AdminService::viewAllAdministrators()
{
    return administratorStore.getAllAdministrators();
}

bool AdminService::addAdministrator(Administrator &admin)
{
    return administratorStore.save(admin);
}

bool AdminService::deleteAdministrator(int adminId)
{
    // Safety check to prevent the last admin from deleting themselves could go here
    return administratorStore.deleteAdministrator(adminId);
}