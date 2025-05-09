#include "auth_service.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

// Constructor: starts logged out
AuthService::AuthService(const std::string& basePath)
  : m_basePath(basePath),
    m_loggedIn(false),
    m_currentId(-1),
    m_currentUsername(""),
    m_currentFullname("")
{}

// Compute next ID by scanning existing file
int AuthService::getNextId() const {
    std::ifstream in(filePath());
    if (!in.good()) return 1;

    std::string line;
    std::getline(in, line);  // skip header

    int maxId = 0;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr;
        if (std::getline(ss, idStr, ',')) {
            int id = std::stoi(idStr);
            if (id > maxId) maxId = id;
        }
    }
    return maxId + 1;
}

// Register a new user; fails if username already exists
bool AuthService::registerUser(const std::string& username,
                               const std::string& password,
                               const std::string& fullName)
{
    std::ifstream in(filePath());
    bool needHeader = !in.good();

    if (!needHeader) {
        std::string line;
        std::getline(in, line);  // skip header
        while (std::getline(in, line)) {
            std::stringstream ss(line);
            std::string idStr, user;
            std::getline(ss, idStr, ',');
            std::getline(ss, user, ',');
            if (user == username) {
                // already exists
                return false;
            }
        }
    }

    // Append to file
    std::ofstream out(filePath(), std::ios::app);
    if (needHeader) {
        out << "id,username,password,fullname\n";
    }
    int newId = getNextId();
    out << newId << ","
        << username << ","
        << password << ","
        << fullName << "\n";
    return true;
}

// Attempt login; sets current user on success
bool AuthService::login(const std::string& username,
                        const std::string& password)
{
    std::ifstream in(filePath());
    if (!in.good()) return false;

    std::string line;
    std::getline(in, line);  // skip header
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string idStr, user, pass, name;
        std::getline(ss, idStr, ',');
        std::getline(ss, user, ',');
        std::getline(ss, pass, ',');
        std::getline(ss, name, ',');

        if (user == username && pass == password) {
            m_currentId       = std::stoi(idStr);
            m_currentUsername = user;
            m_currentFullname = name;
            m_loggedIn        = true;
            return true;
        }
    }
    return false;
}

// Logout current user
void AuthService::logout() {
    m_loggedIn = false;
    m_currentId = -1;
    m_currentUsername.clear();
    m_currentFullname.clear();
}

// Is someone logged in?
bool AuthService::isLoggedIn() const {
    return m_loggedIn;
}

// Getters for current user info
std::string AuthService::getCurrentUsername() const {
    return m_currentUsername;
}
std::string AuthService::getCurrentFullname() const {
    return m_currentFullname;
}

int AuthService::getCurrentUserId() const {
    return m_currentId;
}

std::string AuthService::filePath() const {
    // users.csv inside your base data directory
    return m_basePath + "/users.csv";
}