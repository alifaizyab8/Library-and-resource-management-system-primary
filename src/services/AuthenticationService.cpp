#include "AuthenticationService.h"


#include "../dataStore/AdminUserStore.h"
#include "../dataStore/FinancialStore.h"
#include "../dataStore/AssetStore.h"

// Constructor
AuthenticationService::AuthenticationService(UserStore &userRepo, AdministratorStore &adminRepo)
    : userStore(userRepo), administratorStore(adminRepo) {}


    //Login Methods
std::unique_ptr<Administrator> AuthenticationService::loginAdmin(const std::string &username, const std::string &password)
{

    std::unique_ptr<Administrator> admin = administratorStore.getByUsername(username);

    if(admin !=nullptr && admin->getPassword()==password){
        return admin;
    } 

    return nullptr;
}


std::unique_ptr<User> AuthenticationService::loginUser(const std::string &username, const std::string &password)
{

    std::unique_ptr<User> user = userStore.getByUsername(username);

    if(user !=nullptr && user->getPassword()==password){
        return user;
    }

    return nullptr;
}


// Registration Methods
bool AuthenticationService::registerUser(User &user)
{
    if(userStore.getByUsername(user.getUsername())!= nullptr){
        return false; //already exists
}

    return userStore.save(user);

}


bool AuthenticationService::registerAdmin(Administrator &admin)
{
    if(administratorStore.getByUsername(admin.getUsername())!= nullptr){
        return false; //already exists
    }

    return administratorStore.save(admin);
}