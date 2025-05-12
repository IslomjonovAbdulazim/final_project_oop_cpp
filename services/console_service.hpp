#ifndef CONSOLE_SERVICE_HPP
#define CONSOLE_SERVICE_HPP

#include <string>

class ConsoleService {
public:
    void pause() const;
    std::string prompt(const std::string& msg) const;
    int promptInt(const std::string& msg, int min, int max) const;
    // Print blank lines to clear old output
    void spacer(int lines = 100) const;
};

#endif // CONSOLE_SERVICE_HPP
