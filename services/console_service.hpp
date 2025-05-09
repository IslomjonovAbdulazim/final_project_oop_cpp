#ifndef CONSOLE_SERVICE_HPP
#define CONSOLE_SERVICE_HPP

class ConsoleService {
public:
    ConsoleService() = default;

    // Clear the terminal screen
    void clearScreen() const;

    // Read a single character choice from the user
    char readChoice() const;

    // Pause until the user presses ENTER
    void waitForKey() const;

    // Sleep for the given number of milliseconds
    void sleepMs(int ms) const;
};

#endif // CONSOLE_SERVICE_HPP
