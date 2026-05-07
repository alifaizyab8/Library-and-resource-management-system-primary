#include "AdminUserStore.h"

/* =========================================================
                 ADMINISTRATOR STORE LOGIC
   ========================================================= */

AdministratorStore::AdministratorStore(const std::string &filename) : BaseRepository<Administrator>(filename) {}

bool AdministratorStore::save(Administrator &admin)
{
    if (admin.getAdminId() == 0)
    {
        int newId = dataStore.empty() ? 1 : dataStore.back().getAdminId() + 1;
        admin.setAdminId(newId);
        dataStore.push_back(admin);
    }
    else
    {
        bool found = false;
        for (auto &a : dataStore)
        {
            if (a.getAdminId() == admin.getAdminId())
            {
                a = admin;
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    saveToFile();
    return true;
}

bool AdministratorStore::deleteAdministrator(int adminId)
{
    for (auto it = dataStore.begin(); it != dataStore.end(); ++it)
    {
        if (it->getAdminId() == adminId)
        {
            dataStore.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<Administrator> AdministratorStore::getById(int adminId)
{
    for (const auto &admin : dataStore)
    {
        if (admin.getAdminId() == adminId)
            return std::make_unique<Administrator>(admin);
    }
    return nullptr;
}

std::unique_ptr<Administrator> AdministratorStore::getByUsername(const std::string &username)
{
    for (const auto &admin : dataStore)
    {
        if (admin.getUsername() == username)
            return std::make_unique<Administrator>(admin);
    }
    return nullptr;
}

std::vector<Administrator> AdministratorStore::getAllAdministrators()
{
    return dataStore;
}

/* =========================================================
                     USER STORE LOGIC
   ========================================================= */

UserStore::UserStore(const std::string &filename) : BaseRepository<User>(filename) {}

bool UserStore::save(User &user)
{
    if (user.getUserId() == 0)
    {
        int newId = dataStore.empty() ? 1 : dataStore.back().getUserId() + 1;
        user.setUserId(newId);
        dataStore.push_back(user);
    }
    else
    {
        bool found = false;
        for (auto &u : dataStore)
        {
            if (u.getUserId() == user.getUserId())
            {
                u = user;
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }
    saveToFile();
    return true;
}

bool UserStore::deleteUser(int userId)
{
    for (auto it = dataStore.begin(); it != dataStore.end(); ++it)
    {
        if (it->getUserId() == userId)
        {
            dataStore.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<User> UserStore::getById(int userId)
{
    for (const auto &user : dataStore)
    {
        if (user.getUserId() == userId)
            return std::make_unique<User>(user);
    }
    return nullptr;
}

std::unique_ptr<User> UserStore::getByUsername(const std::string &username)
{
    for (const auto &user : dataStore)
    {
        if (user.getUsername() == username)
            return std::make_unique<User>(user);
    }
    return nullptr;
}

std::vector<User> UserStore::getAllUsers()
{
    return dataStore;
}

std::vector<User> UserStore::getPendingDeletionRequests()
{
    std::vector<User> pending;
    for (const auto &user : dataStore)
    {
        if (user.getDeletionRequested())
            pending.push_back(user);
    }
    return pending;
}