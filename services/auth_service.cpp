#include "auth_service.hpp"
#include <stdexcept>

// Simple hash
static std::string hashPwd(const std::string& pwd) {
    return std::to_string(std::hash<std::string>{}(pwd));
}

AuthService::AuthService(FileManager& fileMgr)
    : fm(fileMgr)
{
    users = fm.loadUsers();
}

User* AuthService::login(const std::string& uname, const std::string& pwd) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it).getUsername() == uname && (*it).verifyPassword(pwd))
            return &(*it);
    }
    return nullptr;
}

User AuthService::signup(const std::string& uname, const std::string& pwd) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it).getUsername() == uname)
            throw std::runtime_error("Username already exists");
    }
    User u(uname, hashPwd(pwd));
    users.add(std::move(u));
    return users.get(users.size()-1);
}

void AuthService::persist() {
    fm.saveUsers(users);
}
