#pragma once
#include "BaseRepository.h"
#include "domain/Administrator.h"
#include "domain/User.h"
#include <memory>
#include <vector>

// admin store
class AdministratorStore : public BaseRepository<Administrator>
{
public:
    explicit AdministratorStore(const std::string &filename);
    bool save(Administrator &admin);
    bool deleteAdministrator(int adminId);
    std::unique_ptr<Administrator> getById(int adminId);
    std::unique_ptr<Administrator> getByUsername(const std::string &username);
    std::vector<Administrator> getAllAdministrators();
};

// user store
class UserStore : public BaseRepository<User>
{
public:
    explicit UserStore(const std::string &filename);
    bool save(User &user);
    bool deleteUser(int userId);
    std::unique_ptr<User> getById(int userId);
    std::unique_ptr<User> getByUsername(const std::string &username);
    std::vector<User> getAllUsers();
    std::vector<User> getPendingDeletionRequests();
};