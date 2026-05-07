#include "AdminUserStore.h"

/* =========================================================
                 ADMINISTRATOR STORE LOGIC
   ========================================================= */

AdministratorStore::AdministratorStore(const std::string &filename) : BaseRepository<Administrator>(filename) {}

bool AdministratorStore::save(Administrator &admin)
{
    if (admin.getAdminId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getAdminId() + 1;
        admin.setAdminId(newId);
        arr.push_back(admin);
    }
    else
    {
        bool found = false;
        for (auto &a : arr)
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
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getAdminId() == adminId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<Administrator> AdministratorStore::getById(int adminId)
{
    for (const auto &admin : arr)
    {
        if (admin.getAdminId() == adminId)
            return std::make_unique<Administrator>(admin);
    }
    return nullptr;
}

std::unique_ptr<Administrator> AdministratorStore::getByUsername(const std::string &username)
{
    for (const auto &admin : arr)
    {
        if (admin.getUsername() == username)
            return std::make_unique<Administrator>(admin);
    }
    return nullptr;
}

std::vector<Administrator> AdministratorStore::getAllAdministrators()
{
    return arr;
}

/* =========================================================
                     USER STORE LOGIC
   ========================================================= */

UserStore::UserStore(const std::string &filename) : BaseRepository<User>(filename) {}

bool UserStore::save(User &user)
{
    if (user.getUserId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getUserId() + 1;
        user.setUserId(newId);
        arr.push_back(user);
    }
    else
    {
        bool found = false;
        for (auto &u : arr)
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
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getUserId() == userId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<User> UserStore::getById(int userId)
{
    for (const auto &user : arr)
    {
        if (user.getUserId() == userId)
            return std::make_unique<User>(user);
    }
    return nullptr;
}

std::unique_ptr<User> UserStore::getByUsername(const std::string &username)
{
    for (const auto &user : arr)
    {
        if (user.getUsername() == username)
            return std::make_unique<User>(user);
    }
    return nullptr;
}

std::vector<User> UserStore::getAllUsers()
{
    return arr;
}

std::vector<User> UserStore::getPendingDeletionRequests()
{
    std::vector<User> pending;
    for (const auto &user : arr)
    {
        if (user.getDeletionRequested())
            pending.push_back(user);
    }
    return pending;
}