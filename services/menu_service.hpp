#ifndef MENU_SERVICE_HPP
#define MENU_SERVICE_HPP

#include "console_service.hpp"
#include "auth_service.hpp"
#include "file_manager.hpp"
#include "../models/folder.hpp"
#include "../models/word.hpp"
#include "../models/quiz.hpp"
#include "../include/list.hpp"

class MenuService {
public:
    MenuService(ConsoleService& console,
                AuthService& auth,
                FileManager& fm);

    // Kick everything off:
    void run();

private:
    ConsoleService& console;
    AuthService&    auth;
    FileManager&    fm;

    // 1) Authentication
    void authMenu();
    void handleLogin();
    void handleRegister();

    // 2) Main app
    void mainMenu();

    // 3) Folder flows
    void folderListMenu();
    void folderDetailMenu(const Folder& folder);
    void newFolderMenu();

    // 4) Word flows
    void newWordMenu();
    void newWordInFolder(const Folder& folder);

    // 5) Quiz flows
    void quizMenu(const Folder& folder);
};

#endif // MENU_SERVICE_HPP
