#include "UserMenu.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// ANSI Color Codes
#define RESET "\033[0m"
#define CYAN "\033[1;36m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"

using namespace std;

// Constructor
UserMenu::UserMenu(UserService &service, int userId)
    : userService(service), loggedInUserId(userId) {}

void UserMenu::displayMenu()
{
    int choice;
    do
    {
        cout << CYAN << "\n=====================================" << RESET << "\n";
        cout << CYAN << "           USER DASHBOARD            " << RESET << "\n";
        cout << CYAN << "=====================================" << RESET << "\n";
        cout << "1. Resource Discovery (Search & View)\n";
        cout << "2. Circulation (Borrow & Return)\n";
        cout << "3. Financials (Fines & Funds)\n";
        cout << "4. Account Management (Profile)\n";
        cout << "0. Logout\n";
        cout << YELLOW << "Enter choice: " << RESET;

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
            handleResourceDiscovery();
            break;
        case 2:
            handleCirculation();
            break;
        case 3:
            handleFinancials();
            break;
        case 4:
            handleAccountManagement();
            break;
        case 0:
            cout << GREEN << "Logging out successfully.\n"
                 << RESET;
            break;
        default:
            cout << RED << "Invalid option. Try again.\n"
                 << RESET;
        }
    } while (choice != 0);
}

void UserMenu::handleResourceDiscovery()
{
    int choice;
    cout << CYAN << "\n--- Resource Discovery ---" << RESET << "\n";
    cout << "1. View Available Resources\n2. Search Resources\n";
    cout << YELLOW << "Choice: " << RESET;
    cin >> choice;

    if (choice == 1)
    {
        auto resources = userService.getAvailableResources();
        if (resources.empty())
            cout << "No resources available currently.\n";
        for (const auto &r : resources)
            cout << r << "\n";
    }
    else if (choice == 2)
    {
        string query;
        cout << YELLOW << "Enter keyword: " << RESET;
        cin >> query;
        auto resources = userService.searchResources(query);
        if (resources.empty())
            cout << "No matches found.\n";
        for (const auto &r : resources)
            cout << r << "\n";
    }
}

void UserMenu::handleCirculation()
{
    int choice;
    cout << CYAN << "\n--- Circulation ---" << RESET << "\n";
    cout << "1. Borrow Resource\n2. Return Resource\n3. Active Borrows\n4. History\n";
    cout << YELLOW << "Choice: " << RESET;
    cin >> choice;

    try
    {
        if (choice == 1)
        {
            int rid;
            cout << YELLOW << "Enter Resource ID to borrow: " << RESET;
            cin >> rid;
            if (userService.borrowResource(loggedInUserId, rid))
                cout << GREEN << "Borrow request sent for Admin approval!\n"
                     << RESET;
        }
        else if (choice == 2)
        {
            int tid;
            string date;
            cout << YELLOW << "Enter Transaction ID: " << RESET;
            cin >> tid;
            cout << YELLOW << "Enter Today's Date (YYYY-MM-DD): " << RESET;
            cin >> date;
            if (userService.returnResource(loggedInUserId, tid, date))
                cout << GREEN << "Resource returned successfully!\n"
                     << RESET;
            else
                cout << RED << "Return failed. Check Transaction ID.\n"
                     << RESET;
        }
        else if (choice == 3)
        {
            auto active = userService.getActiveBorrows(loggedInUserId);
            for (const auto &t : active)
                cout << t << "\n";
        }
        else if (choice == 4)
        {
            auto history = userService.getBorrowingHistory(loggedInUserId);
            for (const auto &h : history)
                cout << h << "\n";
        }
    }
    catch (const exception &e)
    {
        cout << RED << "Error: " << e.what() << RESET << "\n";
    }
}

void UserMenu::handleFinancials()
{
    int choice;
    cout << CYAN << "\n--- Financials ---" << RESET << "\n";
    cout << "1. View Pending Fines\n2. Pay Fine\n3. Submit Fund Request\n4. View Fund Requests\n";
    cout << YELLOW << "Choice: " << RESET;
    cin >> choice;

    try
    {
        if (choice == 1)
        {
            auto fines = userService.getPendingFines(loggedInUserId);
            if (fines.empty())
                cout << GREEN << "You have no pending fines!\n"
                     << RESET;
            for (const auto &f : fines)
                cout << f << "\n";
        }
        else if (choice == 2)
        {
            int fid;
            string date;
            cout << YELLOW << "Enter Fine ID to pay: " << RESET;
            cin >> fid;
            cout << YELLOW << "Enter Today's Date (YYYY-MM-DD): " << RESET;
            cin >> date;
            if (userService.payFine(loggedInUserId, fid, date))
                cout << GREEN << "Fine paid successfully from wallet!\n"
                     << RESET;
        }
        else if (choice == 3)
        {
            double amt;
            string date;
            cout << YELLOW << "Amount to request: " << RESET;
            cin >> amt;
            cout << YELLOW << "Enter Today's Date (YYYY-MM-DD): " << RESET;
            cin >> date;
            if (userService.submitFundRequest(loggedInUserId, amt, date))
                cout << GREEN << "Fund request submitted to Admin!\n"
                     << RESET;
        }
        else if (choice == 4)
        {
            auto requests = userService.getFundRequests(loggedInUserId);
            for (const auto &r : requests)
                cout << r << "\n";
        }
    }
    catch (const exception &e)
    {
        cout << RED << "Error: " << e.what() << RESET << "\n";
    }
}

void UserMenu::handleAccountManagement()
{
    int choice;
    cout << CYAN << "\n--- Account Management ---" << RESET << "\n";
    cout << "1. View Profile\n2. Update Contact Info\n3. Request Account Deletion\n";
    cout << YELLOW << "Choice: " << RESET;
    cin >> choice;

    try
    {
        if (choice == 1)
        {
            auto p = userService.getProfile(loggedInUserId);
            if (p)
                cout << *p << "\n";
        }
        else if (choice == 2)
        {
            string phone, address;
            cout << YELLOW << "New Phone: " << RESET;
            cin >> phone;
            cout << YELLOW << "New Address (No spaces): " << RESET;
            cin >> address;
            if (userService.updateContactInfo(loggedInUserId, phone, address))
                cout << GREEN << "Profile updated!\n"
                     << RESET;
        }
        else if (choice == 3)
        {
            if (userService.requestAccountDeletion(loggedInUserId))
                cout << GREEN << "Account deletion requested. Pending Admin approval.\n"
                     << RESET;
        }
    }
    catch (const exception &e)
    {
        cout << RED << "Error: " << e.what() << RESET << "\n";
    }
}