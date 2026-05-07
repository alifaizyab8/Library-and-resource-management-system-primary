#pragma once
#include "BaseRepository.h"
#include "domain/Administrator.h"
#include "domain/User.h"

class AdminStore : public BaseRepository<Administrator>
{
public:
    AdminStore(const std::string &filename);
    ~AdminStore();
    bool save(Administrator &admin);
    bool deleteAdministrator(int adminId);
    std::unique_ptr<Administrator> getById(int adminId);
    std::unique_ptr<Administrator> getByUsername(const std::string &username);
    std::vector<Administrator> getAllAdministrators();
};

class UserStore: public BaseRepository<User>
{
    
};