#include "services/console_service.hpp"
#include "services/file_manager.hpp"
#include "services/menu_service.hpp"

int main() {
    ConsoleService console;
    FileManager    fm("data");
    MenuService    menu(console, fm);
    menu.run();
    return 0;
}
