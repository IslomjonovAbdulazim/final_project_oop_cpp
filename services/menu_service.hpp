#ifndef MENU_SERVICE_HPP
#define MENU_SERVICE_HPP

#include "console_service.hpp"
#include "file_manager.hpp"
#include "auth_service.hpp"
#include "../models/folder.hpp"
#include "../models/word.hpp"

class MenuService {
private:
    ConsoleService& console;
    FileManager&    fm;
    AuthService     auth;
    User*           currentUser = nullptr;

    void welcome();
    void mainMenu();
    void manageFolders();
    void manageWords(int folderId);
    void takeQuiz();
    void manageQuizResults(int folderId);

public:
    MenuService(ConsoleService& cs, FileManager& fileMgr);
    void run();
};

#endif // MENU_SERVICE_HPP
