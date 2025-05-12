#include "console_service.hpp"
#include <iostream>
#include <limits>

void ConsoleService::pause() const {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string ConsoleService::prompt(const std::string& msg) const {
    std::string input;
    std::cout << msg;
    std::getline(std::cin, input);
    return input;
}

int ConsoleService::promptInt(const std::string& msg, int min, int max) const {
    while (true) {
        std::cout << msg;
        int value;
        if (std::cin >> value && value >= min && value <= max) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Invalid input: please enter a number between "
                  << min << " and " << max << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void ConsoleService::spacer(int lines) const {
    for (int i = 0; i < lines; ++i) {
        std::cout << '\n';
    }
}
