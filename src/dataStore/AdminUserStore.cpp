#include "AdminUserStore.h"
#include <vector>
// Passes the filename up to the BaseRepository template to load the array
AdminStore::AdminStore(const std::string &filename)
    : BaseRepository<Administrator>(filename) {}

AdminStore::~AdminStore() {}

/* *************************************************************************
                 ---------- SAVE ADMINISTRATOR ----------
   ************************************************************************* */

bool AdminStore::save(Administrator &admin)
{
    if (admin.getAdminId() == 0)
    {
        // INSERT: Auto-increment ID by checking the last item in the vector
        int newId = dataStore.empty() ? 1 : dataStore.back().getAdminId() + 1;
        admin.setAdminId(newId);
        dataStore.push_back(admin);
    }
    else
    {
        // UPDATE: Find the existing admin and replace it
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
            return false; // ID not found
    }

    saveToFile(); // Update current state
    return true;
}
bool AdminStore::deleteAdministrator(int adminId)
{
    for (auto it = dataStore.begin(); it != dataStore.end(); ++it)
    {
        if (it->getAdminId() == adminId)
        {
            dataStore.erase(it);
            saveToFile(); // Commit the deletion to the file
            return true;
        }
    }
    return false;
}
std::unique_ptr<Administrator> AdminStore::getById(int adminId)
{
    for (const auto &admin : dataStore)
    {
        if (admin.getAdminId() == adminId)
        {
            return std::make_unique<Administrator>(admin);
        }
    }
    return nullptr;
}
std::unique_ptr<Administrator> AdminStore::getByUsername(const std::string &username)
{
    for (const auto &admin : dataStore)
    {
        if (admin.getUsername() == username)
        {
            return std::make_unique<Administrator>(admin);
        }
    }
    return nullptr;
}
std::vector<Administrator> AdminStore::getAllAdministrators()
{
    // The vector is already populated and held in memory by the BaseRepository
    return dataStore;
}