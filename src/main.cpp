#include <iostream>
#include <string>
#include <fstream>

#include "dataStore/AdminUserStore.h"
#include "dataStore/AssetStore.h"
#include "dataStore/FinancialStore.h"

#include "services/AuthenticationService.h"
#include "services/UserService.h"
#include "services/AdminService.h"

#include "presentation/Session.h"
#include "presentation/AuthMenu.h"
#include "presentation/UserMenu.h"
#include "presentation/AdminMenu.h"

#include "Utility/date.h"

int main()
{
    try
    {
        std::string dataPath = "../src/dataStore/";

        // Initialize data stores
        AdministratorStore adminStore(dataPath + "admins.txt");
        UserStore userStore(dataPath + "users.txt");
        ResourceStore resourceStore(dataPath + "resources.txt");
        CategoryStore categoryStore(dataPath + "categories.txt");
        MembershipTypeStore membershipStore(dataPath + "membership_types.txt");
        BorrowingHistoryStore historyStore(dataPath + "borrowing_history.txt");
        TransactionStore transactionStore(dataPath + "transactions.txt");
        FineStore fineStore(dataPath + "fines.txt");
        FundRequestStore fundStore(dataPath + "fund_requests.txt");

        // Initialize services
        AuthenticationService authService(userStore, adminStore);
        UserService userService(userStore, resourceStore, transactionStore,
                                fineStore, historyStore, fundStore);
        AdminService adminService(userStore, fineStore, resourceStore, categoryStore,
                                  fundStore, transactionStore, membershipStore,
                                  historyStore, adminStore);

        // Get system date and update fines
        std::string systemDate = getCurrentDate();
        adminService.updateDailyFines(systemDate);

        // Main application loop
        AuthMenu authMenu(authService, systemDate);
        bool running = true;

        while (running)
        {
            ActiveSession session = authMenu.displayMenu();

            if (session.isExit)
            {
                running = false;
                std::cout << "\nShutting down. Goodbye!\n";
            }
            else if (session.userId != -1)
            {
                UserMenu userMenu(userService, session.userId, systemDate);
                userMenu.displayMenu();
            }
            else if (session.adminId != -1)
            {
                AdminMenu adminMenu(adminService, systemDate);
                adminMenu.displayDashboard(session.adminId);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}