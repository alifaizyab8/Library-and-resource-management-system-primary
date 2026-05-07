#include <iostream>
#include <string>

#include "dataStore/AdminUserStore.h"
#include "dataStore/AssetStore.h"
#include "dataStore/FinancialStore.h"



// Services
#include "services/AuthenticationService.h"
#include "services/UserService.h"
#include "services/AdminService.h"

// Presentation
#include "presentation/Session.h"
#include "presentation/AuthMenu.h"
#include "presentation/UserMenu.h"
#include "presentation/AdminMenu.h"

// Utilities
#include "Utility/date.h"


int main()
{
    // 1. INITIALIZE DATA STORES (This loads data from the text files into memory)
    AdministratorStore adminStore("admins.txt");
    UserStore userStore("users.txt");

    ResourceStore resourceStore("resources.txt");
    CategoryStore categoryStore("categories.txt");
    MembershipTypeStore membershipStore("membership_types.txt");
    BorrowingHistoryStore historyStore("borrowing_history.txt");

    TransactionStore transactionStore("transactions.txt");
    FineStore fineStore("fines.txt");
    FundRequestStore fundStore("fund_requests.txt");

    // 2. INITIALIZE SERVICES (Dependency Injection)
    AuthenticationService authService(userStore, adminStore);

    UserService userService(userStore, resourceStore, transactionStore,
                            fineStore, historyStore, fundStore);

    // Assuming AdminService constructor looks something like this:
    AdminService adminService(userStore, fineStore, resourceStore, categoryStore,
                              fundStore, transactionStore, membershipStore,
                              historyStore, adminStore);

    // ==========================================
    // 3. MOCK CLOCK & PRE-COMPUTATION
    // ==========================================

    std::cout << "========================================\n";
    std::cout << "   SYSTEM INITIALIZATION (MOCK CLOCK)   \n";
    std::cout << "========================================\n";

    // utility function
    std::string systemDate = getCurrentDate();
    std::cout << "\n[System] Synchronizing database states...\n";
    std::cout << "[System] Calculating overdues and updating daily fines for " << systemDate << "...\n";

    // Pre-computation: Synchronize DB states prior to user interaction
    adminService.updateDailyFines(systemDate);

    std::cout << "[System] Boot sequence complete. Launching interface...\n\n";

    // ==========================================
    // 3. THE OUTER LOOP (Application Lifecycle)
    // ==========================================

    AuthMenu authMenu(authService, systemDate);
    bool running = true;

    while (running)
    {
        // 4. THE AUTHENTICATION GATEWAY (Routing)
        ActiveSession session = authMenu.displayMenu();

        if (session.isExit)
        {
            // Exit signal received
            running = false;
            std::cout << "\nTerminating application safely. Goodbye!\n";
        }
        else if (session.userId != -1)
        {
            // 5. THE INNER LOOP: Route to User Dashboard
            UserMenu userMenu(userService, session.userId, systemDate);
            userMenu.displayMenu(); // Traps execution until User logs out
        }
        else if (session.adminId != -1)
        {
            // 5. THE INNER LOOP: Route to Admin Dashboard
            AdminMenu adminMenu(adminService, systemDate);
            adminMenu.displayDashboard(session.adminId); // Traps execution until Admin logs out
        }
    }

    return 0;
}