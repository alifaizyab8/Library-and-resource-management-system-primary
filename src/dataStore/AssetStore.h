#pragma once
#include "BaseRepository.h"
#include "domain/Resource.h"
#include "domain/Category.h"
#include "domain/MembershipType.h"
#include "domain/BorrowingHistory.h"
#include <memory>
#include <vector>
#include <string>

// resource store
class ResourceStore : public BaseRepository<Resource>
{
public:
    explicit ResourceStore(const std::string &filename);

    bool save(Resource &resource);
    bool deleteResource(int resourceId);

    std::unique_ptr<Resource> getById(int resourceId);
    std::vector<Resource> getAll();
};

// category store
class CategoryStore : public BaseRepository<Category>
{
public:
    explicit CategoryStore(const std::string &filename);

    bool save(Category &category);
    bool deleteCategory(int categoryId);

    std::unique_ptr<Category> getById(int categoryId);
    std::vector<Category> getAll();
};

// membership type store
class MembershipTypeStore : public BaseRepository<MembershipType>
{
public:
    explicit MembershipTypeStore(const std::string &filename);

    bool save(MembershipType &type);
    bool deleteMembershipType(int typeId);

    std::unique_ptr<MembershipType> getById(int typeId);
    std::vector<MembershipType> getAllMembershipTypes();
};

// borowing history store
class BorrowingHistoryStore : public BaseRepository<BorrowingHistory>
{
public:
    explicit BorrowingHistoryStore(const std::string &filename);

    bool save(BorrowingHistory &history);
    bool deleteHistory(int historyId);

    std::unique_ptr<BorrowingHistory> getById(int historyId);
    std::vector<BorrowingHistory> getAll();
    std::vector<BorrowingHistory> getByUserId(int userId);
};