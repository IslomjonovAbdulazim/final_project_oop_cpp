#include "console_service.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

void ConsoleService::clearScreen() const {
#ifdef _WIN32
    // Windows method
    system("cls");
#else
    // For macOS - trying the most reliable approach
    // The 'c' character is appearing in output, suggesting problem with escape sequences
    // Let's try a simple but effective approach first

    // Print 50 newlines to push content off screen
    for (int i = 0; i < 50; i++) {
        std::cout << std::endl;
    }

    // Then try the system command
    std::cout.flush();
    int result = system("clear");

    // Only if system command works, skip the other methods
    // if (result != 0) {
    //     // If system() failed, try direct ANSI codes without std::cout
    //     // Write directly to stdout to avoid any C++ stream buffering issues
    //     write(STDOUT_FILENO, "\033[2J\033[H", 7);
    // }
#endif
}

char ConsoleService::readChoice() const {
    char c;
    std::cin >> c;
    std::cin.ignore(10000, '\n');
    return c;
}

void ConsoleService::waitForKey() const {
    std::cout << "Press ENTER to continue..." << std::flush;
    std::cin.ignore(10000, '\n');
}

void ConsoleService::sleepMs(int ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}