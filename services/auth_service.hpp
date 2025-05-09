#ifndef AUTH_SERVICE_HPP
#define AUTH_SERVICE_HPP

#include <string>
#include "../models/user.hpp"
#include "../include/list.hpp"
#include "file_manager.hpp"

class AuthService {
private:
    List<User> users;
    FileManager& fm;

public:
    AuthService(FileManager& fileMgr);

    User* login(const std::string& uname, const std::string& pwd);
    User  signup(const std::string& uname, const std::string& pwd);
    void  persist();
};

#endif // AUTH_SERVICE_HPP
