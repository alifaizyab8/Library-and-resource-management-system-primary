#pragma once
#include "../domain/Resource.h"
#include "../domain/Category.h"
#include "../domain/User.h"
#include "../domain/Fine.h"
#include "../domain/Administrator.h"
#include "../domain/MembershipType.h"
#include "ConsoleUtils.h"
#include <iostream>

class InputForms
{
public:
    
    static Resource promptForNewResource()
    {
        Resource newResource;

        std::cout << "\n--- ADD NEW RESOURCE ---\n";

        // 1. Safely collect strings
        newResource.setTitle(ConsoleUtils::getValidString("Enter Title: "));
        newResource.setAuthor(ConsoleUtils::getValidString("Enter Author: "));
        newResource.setPublisher(ConsoleUtils::getValidString("Enter Publisher: "));
        newResource.setIsbn(ConsoleUtils::getValidString("Enter ISBN: "));

        // 2. Safely collect integers 
        newResource.setPublicationYear(ConsoleUtils::getValidInt("Enter Publication Year: ", 1000));
        newResource.setCategoryId(ConsoleUtils::getValidInt("Enter Category ID: ", 1));

        int copies = ConsoleUtils::getValidInt("Enter Total Copies: ", 1);
        newResource.setTotalCopies(copies);
        newResource.setAvailableCopies(copies); 

        // 3. Set system defaults
        newResource.setIsActive(true);

        return newResource;
    }

    static void promptForEditResource(Resource &resource)
    {
        std::string tempStr;

        std::cout << "\n--- Current Resource Details ---\n";
        std::cout << "Title: " << resource.getTitle() << "\n";
        std::cout << "Author: " << resource.getAuthor() << "\n";
        std::cout << "Total Copies: " << resource.getTotalCopies() << "\n";
        std::cout << "----------------------------\n";
        std::cout << "Enter new values (or type '.' for strings, or 0 for numbers to keep the current value)\n\n";

        std::cout << "New Title: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            resource.setTitle(tempStr);

        std::cout << "New Author: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            resource.setAuthor(tempStr);

        int tempInt = ConsoleUtils::getValidInt("New Total Copies (current: " + std::to_string(resource.getTotalCopies()) + "): ", 0);
        if (tempInt != 0)
        {
            int difference = tempInt - resource.getTotalCopies();
            resource.setTotalCopies(tempInt);
            resource.setAvailableCopies(resource.getAvailableCopies() + difference);
        }
    }

    static Category promptForNewCategory()
    {
        std::cout << "\n--- ADD NEW CATEGORY ---\n";

        std::string name = ConsoleUtils::getValidString("Enter Category Name: ");
        std::string desc = ConsoleUtils::getValidString("Enter Category Description: ");

        return Category(0, name, desc);
    }

    static void promptForEditCategory(Category &category)
    {
        std::string tempStr;

        std::cout << "\n--- Current Category Details ---\n";
        std::cout << "Name: " << category.getName() << "\n";
        std::cout << "Description: " << category.getDescription() << "\n";
        std::cout << "----------------------------\n";
        std::cout << "Enter new values (or press Enter to keep current)\n\n";

        std::cout << "New Name: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty())
            category.setName(tempStr);

        std::cout << "New Description: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty())
            category.setDescription(tempStr);
    }

    // --- ADD USER ---
    static User promptForNewUser()
    {
        std::cout << "\n--- ADD NEW MEMBER ---\n";

        User newUser;
        newUser.setFirstName(ConsoleUtils::getValidString("Enter First Name: "));
        newUser.setLastName(ConsoleUtils::getValidString("Enter Last Name: "));
        newUser.setUsername(ConsoleUtils::getValidString("Enter Username (min 5 chars): "));
        newUser.setPassword(ConsoleUtils::getValidString("Enter Password: "));
        newUser.setEmail(ConsoleUtils::getValidString("Enter Email: "));
        newUser.setMembershipTypeId(ConsoleUtils::getValidInt("Enter Membership Type ID (1=Basic, 2=Premium): ", 1));

        newUser.setBalance(0.0);
        newUser.setIsActive(true);

        return newUser;
    }

    // --- EDIT USER (Modifies by Reference) ---
    static void promptForEditUser(User &user)
    {
        std::string tempStr;

        std::cout << "\n--- Current User Details ---\n";
        std::cout << "First Name: " << user.getFirstName() << "\n";
        std::cout << "Last Name: " << user.getLastName() << "\n";
        std::cout << "Email: " << user.getEmail() << "\n";
        std::cout << "Membership Type: " << user.getMembershipTypeId() << "\n";
        std::cout << "----------------------------\n";
        std::cout << "Enter new values (or type '.' for strings, or 0 for numbers to keep current)\n\n";

        std::cout << "New First Name: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            user.setFirstName(tempStr);

        std::cout << "New Last Name: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            user.setLastName(tempStr);

        std::cout << "New Email: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            user.setEmail(tempStr);

        std::cout << "New Password (or type '.' to keep current): ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            user.setPassword(tempStr);

        int tempInt = ConsoleUtils::getValidInt("New Membership Type ID (current: " + std::to_string(user.getMembershipTypeId()) + ", enter 0 to keep): ", 0);
        if (tempInt != 0)
        {
            user.setMembershipTypeId(tempInt);
        }
    }

    static Fine promptForNewFine()
    {
        std::cout << "\n--- IMPOSE MANUAL FINE ---\n";

        Fine manualFine;
        manualFine.setUserId(ConsoleUtils::getValidInt("Enter User ID: ", 1));

        // Transaction ID can be 0 for general penalties
        manualFine.setTransactionId(ConsoleUtils::getValidInt("Enter Transaction ID (or 0 if this is a general penalty): ", 0));

        manualFine.setFineAmount(ConsoleUtils::getValidDouble("Enter Fine Amount: $", 0.0));

        manualFine.setDaysOverdue(0);
        manualFine.setIsPaid(false);

        return manualFine;
    }

    static void promptForEditFine(Fine &fine)
    {
        std::cout << "\n--- Current Fine Details ---\n";
        std::cout << fine << "\n";

        double newAmount = ConsoleUtils::getValidDouble("Enter New Fine Amount: $", 0.0);
        fine.setFineAmount(newAmount);
    }

    // --- MEMBERSHIP TYPES ---
    static MembershipType promptForNewMembershipType()
    {
        std::cout << "\n--- ADD MEMBERSHIP TIER ---\n";

        std::string name = ConsoleUtils::getValidString("Tier Name (e.g., Premium): ");
        int duration = ConsoleUtils::getValidInt("Duration (Days, e.g., 365): ", 1);
        double price = ConsoleUtils::getValidDouble("Price ($): ", 0.0);
        int borrowLimit = ConsoleUtils::getValidInt("Max Items Allowed to Borrow: ", 1);
        int borrowDuration = ConsoleUtils::getValidInt("Standard Borrow Duration (Days): ", 1);
        double fine = ConsoleUtils::getValidDouble("Daily Fine for Late Returns ($): ", 0.0);
        std::string desc = ConsoleUtils::getValidString("Description: ");

        return MembershipType(0, name, duration, price, borrowLimit, borrowDuration, fine, desc);
    }

    static void promptForEditMembershipType(MembershipType &tier)
    {
        std::string tempStr;

        std::cout << "\n--- Current Tier Details ---\n";
        std::cout << "Name: " << tier.getMembershipName() << "\n";
        std::cout << "Price: $" << tier.getPrice() << "\n";
        std::cout << "Max Borrowing Limit: " << tier.getMaxBorrowingLimit() << "\n";
        std::cout << "----------------------------\n";
        std::cout << "Enter new values (Type '.' for text or '0' for numbers to keep current)\n\n";

        std::cout << "New Name: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            tier.setMembershipName(tempStr);

        std::cout << "New Description: ";
        std::getline(std::cin, tempStr);
        if (!tempStr.empty() && tempStr != ".")
            tier.setDescription(tempStr);

        double tempDouble = ConsoleUtils::getValidDouble("New Price ($): ", 0.0);
        if (tempDouble != 0)
            tier.setPrice(tempDouble);

        int tempInt = ConsoleUtils::getValidInt("New Max Borrow Limit: ", 0);
        if (tempInt != 0)
            tier.setMaxBorrowingLimit(tempInt);

        tempInt = ConsoleUtils::getValidInt("New Borrow Duration (Days): ", 0);
        if (tempInt != 0)
            tier.setBorrowingDurationDays(tempInt);

        tempDouble = ConsoleUtils::getValidDouble("New Daily Fine ($): ", 0.0);
        if (tempDouble != 0)
            tier.setFinePerDay(tempDouble);
    }

    // --- ADMINISTRATORS ---
    static Administrator promptForNewAdministrator(const std::string &simulatedToday)
    {
        std::cout << "\n--- CREATE SYSTEM ADMINISTRATOR ---\n";
        Administrator newAdmin;

        newAdmin.setFirstName(ConsoleUtils::getValidString("First Name: "));
        newAdmin.setLastName(ConsoleUtils::getValidString("Last Name: "));
        newAdmin.setUsername(ConsoleUtils::getValidString("Username: "));
        newAdmin.setPassword(ConsoleUtils::getValidString("Password: "));
        newAdmin.setEmail(ConsoleUtils::getValidString("Email: "));

        newAdmin.setCreatedDate(simulatedToday);
        newAdmin.setIsActive(true);
        return newAdmin;
    }
};