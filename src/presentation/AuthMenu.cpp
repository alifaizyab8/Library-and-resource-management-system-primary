#include "AuthMenu.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <cstdlib>
#include <limits>

// ANSI Color Codes
#define RESET "\033[0m"
#define CYAN "\033[1;36m"
#define YELLOW "\033[1;33m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"

/* *************************************************************************
                 ---------- CONSTRUCTOR ----------
   ************************************************************************* */

AuthMenu::AuthMenu(AuthenticationService &authService, const std::string &today)
    : authService(authService), dateToday(today)
{
}

/* *************************************************************************
                 ---------- ADMIN LOGIN ----------
   ************************************************************************* */

ActiveSession AuthMenu::handleAdminLogin()
{
    std::string username, password;

    std::cout << CYAN << "\n=== ADMINISTRATOR LOGIN ===\n"
              << RESET;
    std::cout << YELLOW << "Username: " << RESET;
    std::cin >> username;
    std::cout << YELLOW << "Password: " << RESET;
    std::cin >> password;

    std::unique_ptr<Administrator> admin = authService.loginAdmin(username, password);

    if (admin != nullptr)
    {
        if (!admin->getIsActive())
        {
            std::cout << RED << " Error: This admin account is disabled.\n"
                      << RESET;
            return ActiveSession(); // Returns empty session (-1, -1)
        }

        std::cout << GREEN << " Login Successful! Welcome, " << admin->getFirstName() << ".\n"
                  << RESET;

        ActiveSession session;
        session.adminId = admin->getAdminId();
        return session;
    }
    else
    {
        std::cout << RED << " Invalid Username or Password.\n"
                  << RESET;
        return ActiveSession(); // Returns empty session (-1, -1)
    }
}

/* *************************************************************************
                 ---------- USER LOGIN ----------
   ************************************************************************* */

ActiveSession AuthMenu::handleUserLogin()
{
    std::string username, password;

    std::cout << CYAN << "\n=== MEMBER LOGIN ===\n"
              << RESET;
    std::cout << YELLOW << "Username: " << RESET;
    std::cin >> username;
    std::cout << YELLOW << "Password: " << RESET;
    std::cin >> password;

    std::unique_ptr<User> user = authService.loginUser(username, password);

    if (user != nullptr)
    {
        if (!user->getIsActive())
        {
            std::cout << RED << " Error: This account is disabled or suspended. Please contact the administrator.\n"
                      << RESET;
            return ActiveSession(); // Returns empty session (-1, -1)
        }

        std::cout << GREEN << " Login Successful! Welcome, " << user->getFirstName() << ".\n"
                  << RESET;

        ActiveSession session;
        session.userId = user->getUserId();
        return session;
    }
    else
    {
        std::cout << RED << " Invalid Username or Password.\n"
                  << RESET;
        return ActiveSession(); // Returns empty session (-1, -1)
    }
}

/* *************************************************************************
                 ---------- MAIN AUTH GATEWAY ----------
   ************************************************************************* */

ActiveSession AuthMenu::displayMenu()
{
    int choice = -1;
    bool running = true;
    ActiveSession currentSession; // Defaults to: userId = -1, adminId = -1, isExit = false

    while (running)
    {
        ConsoleUtils::clearScreen();

        // Print the Header
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << CYAN << "  LIBRARY AND RESOURCE MANAGEMENT SYSTEM  \n"
                  << RESET;
        std::cout << CYAN << "          Date: " << RESET << dateToday << "\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << "1. Member Login\n";
        std::cout << "2. Administrator Login\n";
        std::cout << "0. Exit System\n";
        std::cout << CYAN << "========================================\n"
                  << RESET;
        std::cout << YELLOW << "Enter your choice: " << RESET;

        // Safe Input Capture
        if (!(std::cin >> choice))
        {
            // The user typed a character instead of an integer. Clear the error state!
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // Silently restart the loop and redraw the menu
        }

        
        switch (choice)
        {
        case 1:
            currentSession = handleUserLogin();
            // If login was successful, break the loop and return to main.cpp
            if (currentSession.userId != -1)
                return currentSession;

            // If login failed, pause the screen so they can read the error
            std::cout << YELLOW << "\nPress Enter to return to main menu..." << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;

        case 2:
            currentSession = handleAdminLogin();
            if (currentSession.adminId != -1)
                return currentSession;

            std::cout << YELLOW << "\nPress Enter to return to main menu..." << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;

        case 0:
            currentSession.isExit = true;
            return currentSession; // Send the exit signal back to main.cpp

        default:
            std::cout << RED << "Invalid choice. Press Enter to try again.\n"
                      << RESET;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            break;
        }
    }

    return currentSession; // Fallback return
}