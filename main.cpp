// src/main.cpp

#include "services/console_service.hpp"
#include "services/auth_service.hpp"
#include "services/file_manager.hpp"
#include "services/menu_service.hpp"

int main() {
    // 1) Console I/O helper
    ConsoleService console;

    // 2) Authentication (stores users in data/users.csv)
    AuthService auth("data");

    // 3) Persistence layer (stores CSVs under data/)
    FileManager fm("data");

    // 4) Menu/navigation glue
    MenuService menu(console, auth, fm);

    // 5) Kick off the whole flow
    menu.run();

    return 0;
}
