#include "AssetStore.h"
// Resource Store
ResourceStore::ResourceStore(const std::string &filename) : BaseRepository<Resource>(filename) {}

bool ResourceStore::save(Resource &resource)
{
    if (resource.getResourceId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getResourceId() + 1;
        resource.setResourceId(newId);
        arr.push_back(resource);
    }
    else
    {
        bool found = false;
        for (auto &r : arr)
        {
            if (r.getResourceId() == resource.getResourceId())
            {
                r = resource;
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

bool ResourceStore::deleteResource(int resourceId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getResourceId() == resourceId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<Resource> ResourceStore::getById(int resourceId)
{
    for (const auto &r : arr)
    {
        if (r.getResourceId() == resourceId)
            return std::make_unique<Resource>(r);
    }
    return nullptr;
}

std::vector<Resource> ResourceStore::getAll()
{
    return arr;
}

// Category Store

CategoryStore::CategoryStore(const std::string &filename) : BaseRepository<Category>(filename) {}
bool CategoryStore::save(Category &category)
{
    if (category.getCategoryId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getCategoryId() + 1;
        category.setCategoryId(newId);
        arr.push_back(category);
    }
    else
    {
        bool found = false;
        for (auto &c : arr)
        {
            if (c.getCategoryId() == category.getCategoryId())
            {
                c = category;
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
bool CategoryStore::deleteCategory(int categoryId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getCategoryId() == categoryId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<Category> CategoryStore::getById(int categoryId)
{
    for (const auto &c : arr)
    {
        if (c.getCategoryId() == categoryId)
            return std::make_unique<Category>(c);
    }
    return nullptr;
}

std::vector<Category> CategoryStore::getAll()
{
    return arr;
}
// Membership Type store
MembershipTypeStore::MembershipTypeStore(const std::string &filename) : BaseRepository<MembershipType>(filename) {}

bool MembershipTypeStore::save(MembershipType &type)
{
    if (type.getMembershipTypeId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getMembershipTypeId() + 1;
        type.setMembershipTypeId(newId);
        arr.push_back(type);
    }
    else
    {
        bool found = false;
        for (auto &t : arr)
        {
            if (t.getMembershipTypeId() == type.getMembershipTypeId())
            {
                t = type;
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

bool MembershipTypeStore::deleteMembershipType(int typeId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getMembershipTypeId() == typeId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<MembershipType> MembershipTypeStore::getById(int typeId)
{
    for (const auto &t : arr)
    {
        if (t.getMembershipTypeId() == typeId)
            return std::make_unique<MembershipType>(t);
    }
    return nullptr;
}

std::vector<MembershipType> MembershipTypeStore::getAllMembershipTypes()
{
    return arr;
}
// Borrowing history Store
BorrowingHistoryStore::BorrowingHistoryStore(const std::string &filename) : BaseRepository<BorrowingHistory>(filename) {}

bool BorrowingHistoryStore::save(BorrowingHistory &history)
{
    if (history.getId() == 0)
    {
        int newId = arr.empty() ? 1 : arr.back().getId() + 1;
        history.setId(newId);
        arr.push_back(history);
    }
    else
    {
        bool found = false;
        for (auto &h : arr)
        {
            if (h.getId() == history.getId())
            {
                h = history;
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

bool BorrowingHistoryStore::deleteHistory(int historyId)
{
    for (auto it = arr.begin(); it != arr.end(); ++it)
    {
        if (it->getId() == historyId)
        {
            arr.erase(it);
            saveToFile();
            return true;
        }
    }
    return false;
}

std::unique_ptr<BorrowingHistory> BorrowingHistoryStore::getById(int historyId)
{
    for (const auto &h : arr)
    {
        if (h.getId() == historyId)
            return std::make_unique<BorrowingHistory>(h);
    }
    return nullptr;
}

std::vector<BorrowingHistory> BorrowingHistoryStore::getAll()
{
    return arr;
}

std::vector<BorrowingHistory> BorrowingHistoryStore::getByUserId(int userId)
{
    std::vector<BorrowingHistory> results;
    for (const auto &h : arr)
    {
        if (h.getUserId() == userId)
            results.push_back(h);
    }
    return results;
}