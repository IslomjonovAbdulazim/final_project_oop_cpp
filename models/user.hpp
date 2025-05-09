#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <ctime>

class User {
private:
    int         id;
    std::string username;
    std::string passwordHash;
    std::time_t createdAt;

    static int nextId;

public:
    User(const std::string& uname, const std::string& pwdHash);
    User(int id_, const std::string& uname, const std::string& pwdHash, std::time_t createdAt_);

    int         getId() const;
    const std::string& getUsername() const;
    bool        verifyPassword(const std::string& pwd) const;  // simple hash compare

    // CSV I/O
    std::string toCsv() const;
    static User   fromCsv(const std::string& line);

    static void   setNextId(int v) { nextId = v; }
};

#endif // USER_HPP
