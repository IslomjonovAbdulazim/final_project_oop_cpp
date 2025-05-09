#include "user.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>

int User::nextId = 1;

static std::string nowIso() {
    std::time_t t = std::time(nullptr);
    std::tm* tm = std::localtime(&t);
    std::ostringstream ss;
    ss << std::put_time(tm, "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

User::User(const std::string& uname, const std::string& pwdHash)
    : id(nextId++), username(uname), passwordHash(pwdHash), createdAt(std::time(nullptr)) {}

User::User(int id_, const std::string& uname, const std::string& pwdHash, std::time_t ct)
    : id(id_), username(uname), passwordHash(pwdHash), createdAt(ct) {}

int User::getId() const { return id; }
const std::string& User::getUsername() const { return username; }
bool User::verifyPassword(const std::string& pwd) const {
    // (in real code, salt+bcrypt; here just compare hashes)
    return std::to_string(std::hash<std::string>{}(pwd)) == passwordHash;
}

std::string User::toCsv() const {
    std::ostringstream ss;
    ss << id << ',' << username << ',' << passwordHash << ',' << createdAt;
    return ss.str();
}

User User::fromCsv(const std::string& line) {
    std::istringstream ss(line);
    std::string tok;
    int id_; std::string uname; std::string ph; std::time_t ct;

    if (!std::getline(ss, tok, ',')) throw std::runtime_error("Bad user csv");
    id_ = std::stoi(tok);
    std::getline(ss, uname, ',');
    std::getline(ss, ph, ',');
    std::getline(ss, tok, ',');
    ct = std::stoll(tok);

    if (id_ >= nextId) nextId = id_ + 1;
    return User(id_, uname, ph, ct);
}
