#pragma once
#include "../services/UserService.h"
#include <string>

class UserMenu
{
private:
    UserService &userService;
    int currentUserId;
    std::string currentDate;
    // sub menu
    void handleResourceDiscovery();
    void handleCirculation();
    void handleFinancials();
    void handleAccountManagement();

public:
    UserMenu(UserService &uService, int userId, const std::string &simulatedDate);

    void displayMenu();
};