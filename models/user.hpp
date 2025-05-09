#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
private:
    static int nextId;     // auto-increment ID

    int       id;
    std::string username;
    std::string password;  // in a real app you'd hash this
    std::string fullname;
    bool      loggedIn;

public:
    // Constructors
    User();
    User(const std::string& username,
         const std::string& password,
         const std::string& fullname);

    // Getters
    int    getId() const;
    std::string getUsername() const;
    std::string getFullname() const;
    bool   isLoggedIn() const;

    // Setters
    void setUsername(const std::string& u);
    void setPassword(const std::string& p);
    void setFullname(const std::string& f);

    // Auth methods
    // Returns true and marks user logged in if password matches
    bool login(const std::string& passwordAttempt);

    // Logs the user out
    void logout();

    // Print brief user info (no password!)
    void printInfo() const;
};

#endif // USER_HPP
