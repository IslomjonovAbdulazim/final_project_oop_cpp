#include "User.hpp"
#include <iostream>

// initialize static counter
int User::nextId = 1;

// ─── Constructors ────────────────────────────────────────────────────────────

User::User()
  : id(nextId++),
    username(""),
    password(""),
    fullname(""),
    loggedIn(false)
{ }

User::User(const std::string& u,
           const std::string& p,
           const std::string& f)
  : id(nextId++),
    username(u),
    password(p),
    fullname(f),
    loggedIn(false)
{ }

// ─── Getters / Setters ───────────────────────────────────────────────────────

int User::getId() const              { return id; }
std::string User::getUsername() const{ return username; }
std::string User::getFullname() const{ return fullname; }
bool User::isLoggedIn() const        { return loggedIn; }

void User::setUsername(const std::string& u) { username = u; }
void User::setPassword(const std::string& p) { password = p; }
void User::setFullname(const std::string& f) { fullname = f; }

// ─── Auth Methods ───────────────────────────────────────────────────────────

bool User::login(const std::string& passwordAttempt) {
    if (passwordAttempt == password) {
        loggedIn = true;
    } else {
        loggedIn = false;
    }
    return loggedIn;
}

void User::logout() {
    loggedIn = false;
}

// ─── Utility ─────────────────────────────────────────────────────────────────

void User::printInfo() const {
    std::cout << "\n=== User Info ===\n"
              << "ID:       " << id << "\n"
              << "Username: " << username << "\n"
              << "Fullname: " << fullname << "\n"
              << "Status:   " << (loggedIn ? "Logged In" : "Logged Out") << "\n"
              << "=================\n";
}
