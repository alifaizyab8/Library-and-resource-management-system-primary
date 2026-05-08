#include "presentation/AdminMenu.h"
#include "presentation/ConsoleUtils.h"
#include "services/AdminService.h"
#include "../validation/validator.h"
#include "presentation/InputForms.h"
#include <iostream>
#include <limits>

// ANSI Color Codes
#define RESET "\033[0m"
#define CYAN "\033[1;36m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"

AdminMenu::AdminMenu(AdminService &service, const std::string &today)
    : adminService(service), simulatedToday(today), currentAdminId(-1) {}

/* *************************************************************************
                    ---------- MAIN DASHBOARD ----------
   ************************************************************************* */

void AdminMenu::displayDashboard(int adminId)
{
    currentAdminId = adminId;
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();

        std::cout << CYAN << "========================================" << RESET << "\n";
        std::cout << CYAN << "          ADMINISTRATOR PORTAL          " << RESET << "\n";
        std::cout << CYAN << "          Date: " << RESET << simulatedToday << "\n";
        std::cout << CYAN << "========================================" << RESET << "\n";
        std::cout << "1.  Catalog Management\n";
        std::cout << "2.  Member Management\n";
        std::cout << "3.  Circulation Desk\n";
        std::cout << "4.  Financial Desk\n";
        std::cout << "5.  Reporting Engine\n";
        std::cout << "6.  System & Admin Settings\n";
        std::cout << "0.  Logout\n";
        std::cout << CYAN << "========================================" << RESET << "\n";
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        try
        {

            switch (choice)
            {
            case 1:
                displayCatalogMenu();
                break;
            case 2:
                displayMemberMenu();
                break;
            case 3:
                displayCirculationMenu();
                break;
            case 4:
                displayFinancialMenu();
                break;
            case 5:
                displayReportingMenu();
                break;
            case 6:
                displaySystemMenu();
                break;
            case 0:
                running = false;
                std::cout << GREEN << "Logging out of Admin Portal...\n"
                          << RESET;
                break;
            default:
                std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                          << RESET;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                break;
            }
        }
        catch (const std::exception &e)
        {
            // Safety used for unexpected system exceptions
            std::cout << RED << "\n========================================\n";
            std::cout << " [CRITICAL SYSTEM ERROR DETECTED] \n";
            std::cout << " Details: " << e.what() << "\n";
            std::cout << " Returning to Main Dashboard to prevent data loss.\n";
            std::cout << "========================================\n"
                      << RESET;

            std::cout << YELLOW << "Press Enter to acknowledge..." << RESET;
            if (std::cin.peek() == '\n')
                std::cin.ignore();
            std::cin.get();
        }
    }
}

/* *************************************************************************
                 ---------- CATALOG MANAGEMENT ----------
   ************************************************************************* */

void AdminMenu::displayCatalogMenu()
{
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();
        std::cout << CYAN << "========================================" << RESET << "\n";
        std::cout << CYAN << "           CATALOG MANAGEMENT           " << RESET << "\n";
        std::cout << CYAN << "========================================" << RESET << "\n";
        std::cout << "1. Add New Resource\n";
        std::cout << "2. Edit Existing Resource\n";
        std::cout << "3. Delete Resource\n";
        std::cout << "4. View All Resources\n";
        std::cout << "5. Add New Category\n";
        std::cout << "6. Edit Category\n";
        std::cout << "7. Delete Category\n";
        std::cout << "8. View All Categories\n";
        std::cout << "0. Back to Main Dashboard\n";
        std::cout << CYAN << "========================================" << RESET << "\n";
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            handleAddResource();
            break;
        case 2:
            handleEditResource();
            break;
        case 3:
            handleDeleteResource();
            break;
        case 4:
            handleViewAllResources();
            break;
        case 5:
            handleAddCategory();
            break;
        case 6:
            handleEditCategory();
            break;
        case 7:
            handleDeleteCategory();
            break;
        case 8:
            handleViewAllCategories();
            break;

        case 0:
            running = false;
            break;
        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }
}

void AdminMenu::handleAddResource()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    Resource newResource = InputForms::promptForNewResource();

    try
    {

        Validator::ValidationResult result = Validator::validate(newResource);

        if (!result.isValid)
        {
            std::cout << RED << "\n Validation Failed:\n"
                      << RESET;
            for (const std::string &error : result.errors)
            {
                std::cout << "  - " << error << "\n";
            }
        }
        else
        {
            if (adminService.addResource(newResource))
            {
                std::cout << GREEN << "\n Resource added successfully!\n"
                          << RESET;
            }
            else
            {
                std::cout << RED << "\n Database Error: Could not add resource.\n"
                          << RESET;
            }
        }
    }

    catch (const std::exception &ex)
    {

        std::cout << RED << "\n [SYSTEM REJECTED DATA]\n";
        std::cout << " Reason: " << ex.what() << "\n";
        std::cout << " The form was aborted to protect database integrity.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleEditResource()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    std::cout << CYAN << "\n--- EDIT RESOURCE ---\n"
              << RESET;
    int resourceId = ConsoleUtils::getValidInt("Enter Resource ID to Edit: ", 1);

    std::unique_ptr<Resource> resource = adminService.getResourceById(resourceId);

    if (!resource)
    {
        std::cout << RED << " Error: Resource ID " << resourceId << " not found.\n"
                  << RESET;
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.get();
        return;
    }

    InputForms::promptForEditResource(*resource);

    try
    {
        Validator::ValidationResult result = Validator::validate(*resource);

        if (!result.isValid)
        {
            std::cout << RED << "\n Validation Failed:\n"
                      << RESET;
            for (const std::string &error : result.errors)
            {
                std::cout << "  - " << error << "\n";
            }
        }
        else
        {
            if (adminService.editResource(*resource))
            {
                std::cout << GREEN << "\n Resource updated successfully!\n"
                          << RESET;
            }
            else
            {
                std::cout << RED << "\n Database Error: Could not update resource.\n"
                          << RESET;
            }
        }
    }
    catch (const std::exception &ex)
    {

        std::cout << RED << "\n [SYSTEM REJECTED DATA]\n";
        std::cout << " Reason: " << ex.what() << "\n";
        std::cout << " The form was aborted to protect database integrity.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleDeleteResource()
{
    std::cout << CYAN << "\n--- DELETE RESOURCE ---\n"
              << RESET;
    int resourceId = ConsoleUtils::getValidInt("Enter Resource ID to delete (or 0 to cancel): ", 0);
    if (resourceId == 0)
        return;

    std::unique_ptr<Resource> targetResource = adminService.getResourceById(resourceId);
    if (targetResource)
    {
        std::cout << "\n Target Resource: " << *targetResource << "\n";
    }
    else
    {
        std::cout << RED << " Error: Resource ID not found.\n"
                  << RESET;
        return;
    }

    std::cout << YELLOW << " WARNING: Are you sure you want to delete this resource? (y/n): " << RESET;
    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        if (adminService.deleteResource(resourceId))
        {
            std::cout << GREEN << " Resource deleted successfully!\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << " Failed to delete resource. It may have active borrowings or reservations.\n"
                      << RESET;
        }
    }
    else
    {
        std::cout << "Deletion cancelled.\n";
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleViewAllResources()
{
    std::cout << CYAN << "\n--- ALL RESOURCES ---\n"
              << RESET;
    std::vector<Resource> resources = adminService.viewAllResources();
    if (resources.empty())
        std::cout << "No resources found.\n";
    for (const auto &r : resources)
    {
        std::cout << r << "\n";
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleAddCategory()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    Category newCategory = InputForms::promptForNewCategory();

    if (adminService.addCategory(newCategory))
    {
        std::cout << GREEN << " Category added successfully!\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Database Error: Could not add category.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleEditCategory()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    std::cout << CYAN << "\n--- EDIT CATEGORY ---\n"
              << RESET;
    int categoryId = ConsoleUtils::getValidInt("Enter Category ID: ", 1);

    std::unique_ptr<Category> category = adminService.getCategoryById(categoryId);
    if (!category)
    {
        std::cout << RED << " Error: Category not found.\n"
                  << RESET;
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.get();
        return;
    }

    InputForms::promptForEditCategory(*category);

    if (adminService.editCategory(*category))
        std::cout << GREEN << " Category updated successfully!\n"
                  << RESET;
    else
        std::cout << RED << " Failed to update category.\n"
                  << RESET;

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}
void AdminMenu::handleDeleteCategory()
{
    std::cout << CYAN << "\n--- DELETE CATEGORY ---\n"
              << RESET;
    int categoryId = ConsoleUtils::getValidInt("Enter Category ID to delete (or 0 to cancel): ", 0);
    if (categoryId == 0)
        return;

    std::unique_ptr<Category> targetCategory = adminService.getCategoryById(categoryId);
    if (targetCategory)
    {
        std::cout << "\n Target Category: " << *targetCategory << "\n";
    }
    else
    {
        std::cout << RED << " Error: Category ID not found.\n"
                  << RESET;
        return;
    }

    std::cout << YELLOW << " Are you sure you want to delete this category? (y/n): " << RESET;
    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        if (adminService.deleteCategory(categoryId))
            std::cout << GREEN << " Category deleted!\n"
                      << RESET;
        else
            std::cout << RED << " Failed to delete. Resources might still be linked to it.\n"
                      << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleViewAllCategories()
{
    std::cout << CYAN << "\n--- ALL CATEGORIES ---\n"
              << RESET;
    std::vector<Category> categories = adminService.viewAllCategories();
    if (categories.empty())
        std::cout << "No categories found.\n";
    for (const auto &c : categories)
    {
        std::cout << c << "\n";
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

/* *************************************************************************
                  ---------- MEMBER MANAGEMENT ----------
   ************************************************************************* */

void AdminMenu::displayMemberMenu()
{
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << CYAN << "           MEMBER MANAGEMENT            \n"
                  << RESET;
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << "1. Add User\n";
        std::cout << "2. Edit User\n";
        std::cout << "3. Delete User\n";
        std::cout << "4. Suspend User\n";
        std::cout << "5. Reactivate User\n";
        std::cout << "6. Process Deletion Requests\n";
        std::cout << "7. View All Users\n";
        std::cout << "0. Back to Main Dashboard\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            handleAddUser();
            break;
        case 2:
            handleEditUser();
            break;
        case 3:
            handleDeleteUser();
            break;
        case 4:
            handleSuspendUser();
            break;
        case 5:
            handleReactivateUser();
            break;
        case 6:
            handleProcessAccountDeletion();
            break;
        case 7:
            handleViewAllUsers();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }
}

void AdminMenu::handleAddUser()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    User newUser = InputForms::promptForNewUser();

    try
    {

        Validator::ValidationResult result = Validator::validate(newUser);

        if (!result.isValid)
        {
            std::cout << RED << "\n Validation Failed:\n"
                      << RESET;
            for (const std::string &error : result.errors)
            {
                std::cout << "  - " << error << "\n";
            }
        }
        else
        {
            if (adminService.addUser(newUser))
            {
                std::cout << GREEN << "\n User added successfully!\n"
                          << RESET;
            }
            else
            {
                std::cout << RED << "\n Database Error: Could not add user (Username or Email might already exist).\n"
                          << RESET;
            }
        }
    }
    catch (const std::exception &ex)
    {

        std::cout << RED << "\n [SYSTEM REJECTED DATA]\n";
        std::cout << " Reason: " << ex.what() << "\n";
        std::cout << " The form was aborted to protect database integrity.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleEditUser()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    std::cout << CYAN << "\n--- EDIT USER ---\n"
              << RESET;
    int userId = ConsoleUtils::getValidInt("Enter User ID to Edit: ", 1);

    std::unique_ptr<User> user = adminService.getUserById(userId);

    if (!user)
    {
        std::cout << RED << " Error: User ID " << userId << " not found.\n"
                  << RESET;
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.get();
        return;
    }

    InputForms::promptForEditUser(*user);

    try
    {

        Validator::ValidationResult result = Validator::validate(*user);

        if (!result.isValid)
        {
            std::cout << RED << "\n Validation Failed:\n"
                      << RESET;
            for (const std::string &error : result.errors)
            {
                std::cout << "  - " << error << "\n";
            }
        }
        else
        {
            if (adminService.editUser(*user))
            {
                std::cout << GREEN << "\n User updated successfully!\n"
                          << RESET;
            }
            else
            {
                std::cout << RED << "\n Database Error: Could not update user. (Check constraints like unique Email).\n"
                          << RESET;
            }
        }
    }
    catch (const std::exception &ex)
    {

        std::cout << RED << "\n [SYSTEM REJECTED DATA]\n";
        std::cout << " Reason: " << ex.what() << "\n";
        std::cout << " The form was aborted to protect database integrity.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleDeleteUser()
{
    std::cout << CYAN << "\n--- DELETE USER ---\n"
              << RESET;
    int userId = ConsoleUtils::getValidInt("Enter User ID to delete (or 0 to cancel): ", 0);
    if (userId == 0)
        return;

    std::unique_ptr<User> targetUser = adminService.getUserById(userId);
    if (targetUser)
    {
        std::cout << "\n Target User: " << *targetUser << "\n";
    }
    else
    {
        std::cout << RED << " Error: User ID not found.\n"
                  << RESET;
        return;
    }

    std::cout << YELLOW << " WARNING: This will permanently delete the user. Are you sure? (y/n): " << RESET;
    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        if (adminService.deleteUserAccount(userId))
        {
            std::cout << GREEN << " User permanently deleted.\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << " Failed to delete user. They may have active borrowed books or unpaid fines.\n"
                      << RESET;
        }
    }
    else
    {
        std::cout << "Deletion cancelled.\n";
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleSuspendUser()
{
    int userId;
    std::cout << CYAN << "\n--- SUSPEND USER ACCOUNT ---\n"
              << RESET;
    userId = ConsoleUtils::getValidInt("Enter User ID to Suspend (or 0 to cancel): ", 0);
    if (userId == 0)
        return;

    if (adminService.suspendUserAccount(userId))
    {
        std::cout << GREEN << " User account suspended successfully.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Failed to suspend account. User ID might not exist.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleReactivateUser()
{
    int userId;
    std::cout << CYAN << "\n--- REACTIVATE USER ACCOUNT ---\n"
              << RESET;
    userId = ConsoleUtils::getValidInt("Enter User ID to Reactivate (or 0 to cancel): ", 0);
    if (userId == 0)
        return;

    if (adminService.reactivateUserAccount(userId))
    {
        std::cout << GREEN << " User account reactivated successfully.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Failed to reactivate account. User ID might not exist.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleProcessAccountDeletion()
{
    std::cout << CYAN << "\n--- PROCESS DELETION REQUESTS ---\n"
              << RESET;

    std::vector<User> pendingRequests = adminService.viewDeletionRequests();

    if (pendingRequests.empty())
    {
        std::cout << "No pending deletion requests at this time.\n";
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    for (const User &u : pendingRequests)
    {
        std::cout << u << "\n";
    }

    int userId;
    char decision;

    userId = ConsoleUtils::getValidInt("\nEnter User ID to process (or 0 to cancel): ", 0);
    if (userId == 0)
        return;
    std::cout << YELLOW << "Approve deletion for User ID " << userId << "? (y/n): " << RESET;
    std::cin >> decision;

    bool approve = (decision == 'y' || decision == 'Y');

    if (adminService.processAccountDeletionRequest(userId, approve))
    {
        std::cout << GREEN << " Deletion request " << (approve ? "APPROVED and user deleted" : "REJECTED and request cleared") << ".\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Error: Could not process request. Please check the User ID.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleViewAllUsers()
{
    std::cout << CYAN << "\n--- ALL REGISTERED MEMBERS ---\n"
              << RESET;
    std::vector<User> users = adminService.viewAllUsers();

    if (users.empty())
    {
        std::cout << "No users found in the system.\n";
    }
    else
    {
        for (const User &u : users)
        {
            std::cout << u << "\n";
        }
    }
    std::cout << YELLOW << "\nPress Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

/* *************************************************************************
                  ---------- CIRCULATION DESK ----------
   ************************************************************************* */

void AdminMenu::displayCirculationMenu()
{
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << CYAN << "           CIRCULATION DESK             \n"
                  << RESET;
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << "1. View Pending Borrow Requests\n";
        std::cout << "2. Process a Borrow Request\n";
        std::cout << "3. Process a Book Return\n";
        std::cout << "4. View All Transactions\n";
        std::cout << "5. View Transactions By User\n";
        std::cout << "0. Back to Main Dashboard\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
        {

            std::vector<Transaction> pending = adminService.viewPendingBorrowRequests();
            std::cout << CYAN << "\n--- Pending Requests ---\n"
                      << RESET;
            if (pending.empty())
            {
                std::cout << "No pending requests found.\n";
            }
            else
            {
                for (const auto &txn : pending)
                {
                    std::cout << "Txn ID: " << txn.getTransactionId()
                              << " | User ID: " << txn.getUserId()
                              << " | Resource ID: " << txn.getResourceId() << "\n";
                }
            }
            std::cout << YELLOW << "\nPress Enter to continue..." << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        break;

        case 2:
            handleProcessBorrowRequest();
            break;

        case 3:
            handleProcessReturn();
            break;
        case 4:
            handleViewAllTransactions();
            break;
        case 5:
            handleViewUserTransactions();
            break;
        case 0:
            running = false;
            break;

        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }
}

void AdminMenu::handleProcessBorrowRequest()
{
    int txnId;
    char decision;

    txnId = ConsoleUtils::getValidInt("\nEnter Transaction ID to Process (or 0 to cancel): ", 0);
    if (txnId == 0)
        return;
    std::cout << YELLOW << "Approve this request? (y/n): " << RESET;
    std::cin >> decision;

    bool approve = (decision == 'y' || decision == 'Y');

    bool success = adminService.processBorrowRequest(txnId, approve, simulatedToday);

    if (success)
    {
        std::cout << GREEN << " Request " << (approve ? "APPROVED" : "REJECTED") << " successfully.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Error: Could not process request. (Resource might be out of stock).\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleProcessReturn()
{
    int txnId;
    txnId = ConsoleUtils::getValidInt("\nEnter Transaction ID to Process Return (or 0 to cancel): ", 0);
    if (txnId == 0)
        return;

    bool success = adminService.processReturn(txnId, simulatedToday);

    if (success)
    {
        std::cout << GREEN << " Book returned successfully. Inventory updated and fines calculated.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Error: Could not process return. Check if the Transaction ID is correct and currently ISSUED.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleViewAllTransactions()
{
    std::cout << CYAN << "\n--- ALL TRANSACTIONS ---\n"
              << RESET;
    std::vector<Transaction> txns = adminService.viewAllTransactions();
    if (txns.empty())
        std::cout << "No transactions found.\n";
    for (const auto &t : txns)
    {
        std::cout << t << "\n";
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleViewUserTransactions()
{
    int userId = ConsoleUtils::getValidInt("Enter User ID to view transactions for: ", 1);

    std::cout << CYAN << "\n--- TRANSACTIONS FOR USER " << userId << " ---\n"
              << RESET;
    std::vector<Transaction> txns = adminService.viewTransactionsByUser(userId);
    if (txns.empty())
        std::cout << "No transactions found for this user.\n";
    for (const auto &t : txns)
    {
        std::cout << t << "\n";
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

/* *************************************************************************
                    ---------- FINANCIAL DESK ----------
   ************************************************************************* */

void AdminMenu::displayFinancialMenu()
{
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << CYAN << "           FINANCIAL DESK               \n"
                  << RESET;
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << "1. Process Fund Requests\n";
        std::cout << "2. View Fines by User\n";
        std::cout << "3. Receive Fine Payment\n";
        std::cout << "4. View All System Fines\n";
        std::cout << "5. Impose Manual Fine\n";
        std::cout << "6. Edit Existing Fine\n";
        std::cout << "7. Delete Fine Record\n";
        std::cout << "0. Back to Main Dashboard\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            handleProcessFundRequest();
            break;
        case 2:
            handleViewUserFines();
            break;
        case 3:
            handleReceiveFinePayment();
            break;
        case 4:
            handleViewAllFines();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }
}

void AdminMenu::handleProcessFundRequest()
{
    std::cout << CYAN << "\n--- PROCESS FUND REQUESTS ---\n"
              << RESET;

    std::vector<FundRequest> pendingRequests = adminService.viewPendingFundRequests();

    if (pendingRequests.empty())
    {
        std::cout << "No pending fund requests at this time.\n";
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    for (const FundRequest &req : pendingRequests)
    {
        if (req.getStatus() == "PENDING")
        {
            std::cout << req << "\n";
        }
    }

    int reqId;
    char decision;

    reqId = ConsoleUtils::getValidInt("\nEnter Request ID to process (or 0 to cancel): ", 0);
    if (reqId == 0)
        return;

    bool found = false;
    for (const FundRequest &req : pendingRequests)
    {
        if (req.getRequestId() == reqId && req.getStatus() == "PENDING")
        {
            std::cout << "\n Selected Request: " << req << "\n";
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << RED << " Error: Invalid Request ID or request is not pending.\n"
                  << RESET;
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return;
    }

    std::cout << YELLOW << "Approve this fund request? (y/n): " << RESET;
    std::cin >> decision;

    bool approve = (decision == 'y' || decision == 'Y');

    if (adminService.processFundRequest(reqId, approve, simulatedToday))
    {
        std::cout << GREEN << " Fund request " << (approve ? "APPROVED. Balance updated." : "REJECTED.") << "\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Error: Could not process request. Please check the Request ID.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleViewUserFines()
{
    int userId;
    std::cout << CYAN << "\n--- VIEW USER FINES ---\n"
              << RESET;
    userId = ConsoleUtils::getValidInt("Enter User ID (or 0 to cancel): ", 0);
    if (userId == 0)
        return;

    std::vector<Fine> userFines = adminService.viewFinesByUser(userId);

    if (userFines.empty())
    {
        std::cout << " No fines found for User ID " << userId << ".\n";
    }
    else
    {
        std::cout << CYAN << "\n--- Fines for User ID " << userId << " ---\n"
                  << RESET;
        for (const Fine &fine : userFines)
        {
            std::cout << fine << "\n";
        }
    }

    std::cout << YELLOW << "\nPress Enter to continue..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void AdminMenu::handleReceiveFinePayment()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    std::cout << CYAN << "\n--- RECEIVE FINE PAYMENT ---\n"
              << RESET;
    int fineId = ConsoleUtils::getValidInt("Enter Fine ID to mark as PAID: ", 1);

    if (adminService.markFineAsPaid(fineId))
    {
        std::cout << GREEN << " Payment received! Fine marked as Paid.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Error: Could not process payment. Verify the Fine ID.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleViewAllFines()
{
    std::cout << CYAN << "\n--- ALL SYSTEM FINES ---\n"
              << RESET;
    std::vector<Fine> fines = adminService.viewAllFines();

    if (fines.empty())
    {
        std::cout << "No fines exist in the system.\n";
    }
    else
    {
        for (const Fine &f : fines)
        {
            std::cout << f << "\n";
        }
    }
    std::cout << YELLOW << "\nPress Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleImposeFine()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    Fine manualFine = InputForms::promptForNewFine();
    manualFine.setFineDate(simulatedToday);

    if (adminService.imposeFine(manualFine))
    {
        std::cout << GREEN << " Manual fine imposed successfully!\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Database Error: Could not impose fine. Check if User ID exists.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleUpdateFine()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    std::cout << CYAN << "\n--- EDIT EXISTING FINE ---\n"
              << RESET;
    int fineId = ConsoleUtils::getValidInt("Enter Fine ID to Edit: ", 1);

    std::unique_ptr<Fine> targetFine = adminService.getFineById(fineId);

    if (!targetFine)
    {
        std::cout << RED << " Error: Fine ID " << fineId << " not found.\n"
                  << RESET;
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.get();
        return;
    }

    InputForms::promptForEditFine(*targetFine);

    if (adminService.updateFine(*targetFine))
    {
        std::cout << GREEN << " Fine updated successfully to $" << targetFine->getFineAmount() << "!\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Database Error: Could not update fine.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleDeleteFine()
{
    std::cout << CYAN << "\n--- DELETE FINE ---\n"
              << RESET;
    int fineId = ConsoleUtils::getValidInt("Enter Fine ID to delete (or 0 to cancel): ", 0);
    if (fineId == 0)
        return;

    std::unique_ptr<Fine> targetFine = adminService.getFineById(fineId);
    if (targetFine)
    {
        std::cout << "\n Target Fine: " << *targetFine << "\n";
    }
    else
    {
        std::cout << RED << " Error: Fine ID not found.\n"
                  << RESET;
        return;
    }

    std::cout << YELLOW << "  WARNING: This permanently erases the fine from the database. \n";
    std::cout << "   (If you just want to forgive the debt, use 'Waive Fine' instead).\n";
    std::cout << "   Are you sure? (y/n): " << RESET;

    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        if (adminService.deleteFine(fineId))
        {
            std::cout << GREEN << " Fine record permanently deleted.\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << " Error: Could not delete fine. Verify the Fine ID.\n"
                      << RESET;
        }
    }
    else
    {
        std::cout << "Action cancelled.\n";
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

/* *************************************************************************
                   ---------- REPORTING ENGINE ----------
   ************************************************************************* */

void AdminMenu::displayReportingMenu()
{
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << CYAN << "           REPORT GENERATION MENU       \n"
                  << RESET;
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << "1. User Borrowing History Report\n";
        std::cout << "2. Issued/Overdue Resources Report\n";
        std::cout << "0. Back to Main Dashboard\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            handleGenerateHistoryReport();
            break;
        case 2:
            handleGenerateIssue_OverdueReport();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }
}

void AdminMenu::handleGenerateHistoryReport()
{
    std::string filename;

    std::cout << CYAN << "\n--- USER BORROWING HISTORY REPORT ---\n"
              << RESET;
    std::cout << YELLOW << "Enter the name of the file to save (e.g., HistoryReport): " << RESET;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, filename);

    if (filename.empty())
    {
        filename = "UserHistoryReport";
        std::cout << "No filename provided. Defaulting to: " << filename << "\n";
    }

    try
    {
        if (adminService.generateUserHistoryReport(filename))
        {
            std::cout << GREEN << " Report generated successfully!\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << " Error while generating report. Please check file permissions and try again.\n"
                      << RESET;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << RED << "\n [FILE SYSTEM ERROR]\n";
        std::cout << " Could not generate Report: " << e.what() << "\n";
        std::cout << " Hint: Make sure the file isn't already open in another program!\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleGenerateIssue_OverdueReport()
{
    std::string filename;

    std::cout << CYAN << "\n--- ISSUED & OVERDUE RESOURCES REPORT ---\n"
              << RESET;
    std::cout << YELLOW << "Enter the name of the file to save (e.g., OverdueReport): " << RESET;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, filename);

    if (filename.empty())
    {
        filename = "OverdueReport";
        std::cout << "No filename provided. Defaulting to: " << filename << "\n";
    }

    try
    {
        if (adminService.generateIssuedAndOverdueReport(filename))
        {
            std::cout << GREEN << " Report generated successfully!\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << " Error while generating report. Please check file permissions and try again.\n"
                      << RESET;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << RED << "\n [FILE SYSTEM ERROR]\n";
        std::cout << " Could not generate Report: " << e.what() << "\n";
        std::cout << " Hint: Make sure the file isn't already open in another program!\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

/* *************************************************************************
                 ---------- SYSTEM & ADMIN SETTINGS ----------
   ************************************************************************* */

void AdminMenu::displaySystemMenu()
{
    int choice = -1;
    bool running = true;

    while (running)
    {
        ConsoleUtils::clearScreen();
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << CYAN << "       SYSTEM & ADMIN SETTINGS          \n"
                  << RESET;
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << "--- Membership Tier Management ---\n";
        std::cout << "1. View All Membership Types\n";
        std::cout << "2. Add Membership Type\n";
        std::cout << "3. Edit Membership Type\n";
        std::cout << "4. Delete Membership Type\n";
        std::cout << "\n--- Administrator Management ---\n";
        std::cout << "5. View All Administrators\n";
        std::cout << "6. Add New Administrator\n";
        std::cout << "7. Delete Administrator Account\n";
        std::cout << "0. Back to Main Dashboard\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << YELLOW << "Enter your choice: " << RESET;

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            handleViewAllMembershipTypes();
            break;
        case 2:
            handleAddMembershipType();
            break;
        case 3:
            handleEditMembershipType();
            break;
        case 4:
            handleDeleteMembershipType();
            break;
        case 5:
            handleViewAllAdministrators();
            break;
        case 6:
            handleAddAdministrator();
            break;
        case 7:
            handleDeleteAdministrator();
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(10000, '\n');
            std::cin.get();
            break;
        }
    }
}

void AdminMenu::handleViewAllMembershipTypes()
{
    std::cout << CYAN << "\n--- MEMBERSHIP TIERS ---\n"
              << RESET;
    std::vector<MembershipType> types = adminService.viewAllMembershipTypes();
    if (types.empty())
        std::cout << "No membership tiers found.\n";
    for (const auto &t : types)
    {
        std::cout << t << "\n";
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleAddMembershipType()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    MembershipType newTier = InputForms::promptForNewMembershipType();

    if (adminService.addMembershipType(newTier))
        std::cout << GREEN << " Tier added successfully!\n"
                  << RESET;
    else
        std::cout << RED << " Database Error.\n"
                  << RESET;

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleEditMembershipType()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    int typeId = ConsoleUtils::getValidInt("Enter Membership Type ID to edit: ", 1);

    std::unique_ptr<MembershipType> tier = adminService.getMembershipTypeById(typeId);

    if (!tier)
    {
        std::cout << RED << " Error: Membership Type ID " << typeId << " not found.\n"
                  << RESET;
        std::cout << YELLOW << "Press Enter to continue..." << RESET;
        std::cin.get();
        return;
    }

    InputForms::promptForEditMembershipType(*tier);

    if (adminService.editMembershipType(*tier))
    {
        std::cout << GREEN << "\n Membership Tier updated successfully!\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << "\n Database Error: Could not update tier.\n"
                  << RESET;
    }

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleDeleteMembershipType()
{
    std::cout << CYAN << "\n--- DELETE MEMBERSHIP TYPE ---\n"
              << RESET;
    int typeId = ConsoleUtils::getValidInt("Enter Membership Type ID to delete (or 0 to cancel): ", 0);
    if (typeId == 0)
        return;

    std::unique_ptr<MembershipType> targetTier = adminService.getMembershipTypeById(typeId);
    if (targetTier)
    {
        std::cout << "\n Target Tier: " << *targetTier << "\n";
    }
    else
    {
        std::cout << RED << " Error: Membership Type ID not found.\n"
                  << RESET;
        return;
    }

    std::cout << YELLOW << " Proceed with deletion? (y/n): " << RESET;
    char confirm;
    std::cin >> confirm;

    if ((confirm == 'y' || confirm == 'Y') && adminService.deleteMembershipType(typeId))
    {
        std::cout << GREEN << " Tier deleted.\n"
                  << RESET;
    }
    else
    {
        std::cout << RED << " Deletion failed or cancelled.\n"
                  << RESET;
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleViewAllAdministrators()
{
    std::cout << CYAN << "\n--- SYSTEM ADMINISTRATORS ---\n"
              << RESET;
    std::vector<Administrator> admins = adminService.viewAllAdministrators();
    if (admins.empty())
        std::cout << "No admins found.\n";
    for (const auto &a : admins)
    {
        std::cout << a << "\n";
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void AdminMenu::handleAddAdministrator()
{
    if (std::cin.peek() == '\n')
        std::cin.ignore();

    Administrator newAdmin = InputForms::promptForNewAdministrator(simulatedToday);

    if (adminService.addAdministrator(newAdmin))
        std::cout << GREEN << " Admin account created!\n"
                  << RESET;
    else
        std::cout << RED << " Failed. Username or Email might be taken.\n"
                  << RESET;

    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.get();
}

void AdminMenu::handleDeleteAdministrator()
{
    std::cout << CYAN << "\n--- DELETE ADMINISTRATOR ---\n"
              << RESET;
    int adminId = ConsoleUtils::getValidInt("Enter Admin ID to delete (or 0 to cancel): ", 0);
    if (adminId == 0)
        return;

    if (adminId == currentAdminId)
    {
        std::cout << RED << " You cannot delete your own account while logged in!\n"
                  << RESET;
    }
    else
    {
        std::cout << YELLOW << " Are you sure? (y/n): " << RESET;
        char confirm;
        std::cin >> confirm;
        if ((confirm == 'y' || confirm == 'Y') && adminService.deleteAdministrator(adminId))
        {
            std::cout << GREEN << " Admin deleted.\n"
                      << RESET;
        }
        else
        {
            std::cout << RED << " Deletion failed or cancelled.\n"
                      << RESET;
        }
    }
    std::cout << YELLOW << "Press Enter to continue..." << RESET;
    std::cin.ignore(10000, '\n');
    std::cin.get();
}