#ifndef AUTH_SERVICE_HPP
#define AUTH_SERVICE_HPP

#include <string>

class AuthService {
public:
    explicit AuthService(const std::string& basePath = "data");

    bool registerUser(const std::string& username,
                      const std::string& password,
                      const std::string& fullName);
    bool login(const std::string& username,
               const std::string& password);
    void logout();
    bool isLoggedIn() const;

    std::string getCurrentUsername() const;
    std::string getCurrentFullname() const;
    int         getCurrentUserId() const;    // ← new

private:
    std::string m_basePath;
    bool        m_loggedIn;
    int         m_currentId;
    std::string m_currentUsername;
    std::string m_currentFullname;

    std::string filePath() const;
    int         getNextId() const;

};

#endif // AUTH_SERVICE_HPP
