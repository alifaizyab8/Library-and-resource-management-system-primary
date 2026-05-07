#pragma once
#include "../services/UserService.h"
#include <string>

class UserMenu
{
private:
    UserService &userService;
    int loggedInUserId;
    // sub menu
    void handleResourceDiscovery();
    void handleCirculation();
    void handleFinancials();
    void handleAccountManagement();

public:
    UserMenu(UserService &service, int userId);

    void displayMenu();
};