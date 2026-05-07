#pragma once

#include <string>
#include <memory>
// Include the domain models 
#include "../domain/User.h"
#include "../domain/Administrator.h"

// Forward declarations
class UserStore;
class AdministratorStore;

class AuthenticationService {
    private:
        UserStore &userStore;
        AdministratorStore &administratorStore;

    public:
        AuthenticationService(UserStore &userRepo, AdministratorStore &adminRepo);

        // Authentication Methods
        std::unique_ptr<User> loginUser(const std::string &username, const std::string &password);
        std::unique_ptr<Administrator> loginAdmin(const std::string &username, const std::string &password);

        // Create new user/admin accounts (if needed)
        bool registerUser(User &user);
        bool registerAdmin(Administrator &admin);


};