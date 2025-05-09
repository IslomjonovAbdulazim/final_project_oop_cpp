#include "menu_service.hpp"
#include <iostream>
#include <chrono>

// ——— Constructor & run ———————————————————————————————————————————————

MenuService::MenuService(ConsoleService& console_,
                         AuthService& auth_,
                         FileManager& fm_)
  : console(console_), auth(auth_), fm(fm_) {}

void MenuService::run() {
    authMenu();
    mainMenu();
}

// ——— Authentication ——————————————————————————————————————————————————

void MenuService::authMenu() {
    while (!auth.isLoggedIn()) {
        console.clearScreen();
        std::cout
          << "1) Login\n"
             "2) Register\n"
             "x) Exit\n\n"
             "Enter choice: ";
        char c = console.readChoice();
        switch (c) {
            case '1': handleLogin();    break;
            case '2': handleRegister(); break;
            case 'x': std::exit(0);
            default:  break;
        }
    }
}

void MenuService::handleLogin() {
    console.clearScreen();
    std::string u, p;
    std::cout << "Username: ";
    std::getline(std::cin, u);
    std::cout << "Password: ";
    std::getline(std::cin, p);

    if (auth.login(u, p)) {
        std::cout << "\nWelcome, " << auth.getCurrentFullname() << "!\n";
    } else {
        std::cout << "\nInvalid credentials.\n";
    }
    console.waitForKey();
}

void MenuService::handleRegister() {
    console.clearScreen();
    std::string u, p, f;
    std::cout << "Choose username: ";
    std::getline(std::cin, u);
    std::cout << "Choose password: ";
    std::getline(std::cin, p);
    std::cout << "Full name: ";
    std::getline(std::cin, f);

    if (auth.registerUser(u, p, f)) {
        std::cout << "\nAccount created! Please log in.\n";
    } else {
        std::cout << "\nThat username is already taken.\n";
    }
    console.waitForKey();
}

// ——— Main Menu ————————————————————————————————————————————————————

void MenuService::mainMenu() {
    while (auth.isLoggedIn()) {
        console.clearScreen();
        std::cout
          << "📚  Main Menu\n"
             "a) Start Quiz\n"
             "b) Folders\n"
             "c) New Folder\n"
             "d) New Word\n"
             "l) Logout\n\n"
             "Enter choice: ";
        char c = console.readChoice();
        switch (c) {
            case 'a': folderListMenu(); break;  // pick a folder, then quiz
            case 'b': folderListMenu(); break;
            case 'c': newFolderMenu();   break;
            case 'd': newWordMenu();     break;
            case 'l': auth.logout();     break;
            default:                      break;
        }
        if (auth.isLoggedIn()) console.waitForKey();
    }
}

// ——— Folder List & Detail ——————————————————————————————————————————

void MenuService::folderListMenu() {
    console.clearScreen();
    auto all = fm.loadFolders();
    List<Folder> mine;
    int uid = auth.getCurrentUserId();

    for (auto it = all.begin(); it != all.end(); ++it) {
        if ((*it).getUserId() == uid) mine.push_back(*it);
    }

    if (mine.begin() == mine.end()) {
        std::cout << "No folders yet.\n"
                  << "Press c to create a new folder, or h for Home\n";
        char c = console.readChoice();
        if (c == 'c') newFolderMenu();
        return;
    }

    // show numbered list
    std::cout << "Your folders:\n";
    for (int i = 0; i < mine.size(); ++i) {
        std::cout << (i+1) << ") " << mine[i].getName() << "\n";
    }
    std::cout << "Select (1-" << mine.size()
              << "), c=New Folder, h=Home: ";
    char c = console.readChoice();
    if (c == 'c') {
        newFolderMenu();
    } else if (c == 'h') {
        return;
    } else if (isdigit(c)) {
        int idx = c - '1';
        if (idx >= 0 && idx < mine.size()) {
            folderDetailMenu(mine[idx]);
        }
    }
}

void MenuService::folderDetailMenu(const Folder& f) {
    console.clearScreen();
    f.printInfo();
    std::cout
      << "a) Delete Folder\n"
         "b) Rename Folder\n"
         "c) Stats\n"
         "d) New Word\n"
         "e) Start Quiz\n"
         "h) Home\n\n"
         "Enter choice: ";
    char c = console.readChoice();

    switch (c) {
        case 'a': {
            console.clearScreen();
            std::cout << "Type YES to confirm deletion: ";
            std::string confirm;
            std::getline(std::cin, confirm);
            if (confirm == "YES") {
                fm.deleteFolder(f.getId());
                std::cout << "Folder deleted.\n";
                console.waitForKey();
                return;
            }
            break;
        }
        case 'b': {
            console.clearScreen();
            std::cout << "New name: ";
            std::string nn;
            std::getline(std::cin, nn);
            Folder copy = f;
            copy.setName(nn);
            fm.updateFolder(copy);
            std::cout << "Renamed.\n";
            console.waitForKey();
            return;
        }
        case 'c':
            console.clearScreen();
            f.printInfo();
            console.waitForKey();
            return;
        case 'd':
            newWordInFolder(f);
            return;
        case 'e':
            quizMenu(f);
            return;
        case 'h':
            return;
        default:
            break;
    }
    // any other key: go back to detail
    folderDetailMenu(f);
}

// ——— New Folder ————————————————————————————————————————————————————

void MenuService::newFolderMenu() {
    console.clearScreen();
    std::cout << "🔖  Create New Folder\n";
    std::cout << "Name: ";
    std::string name;
    std::getline(std::cin, name);
    std::cout << "Description: ";
    std::string desc;
    std::getline(std::cin, desc);

    Folder f(auth.getCurrentUserId(), name, desc);
    auto all = fm.loadFolders();
    all.push_back(f);
    fm.saveFolders(all);

    std::cout << "Folder created!\n";
    console.waitForKey();
}

// ——— New Word —————————————————————————————————————————————————————

void MenuService::newWordMenu() {
    console.clearScreen();
    std::cout << "✏️  Add New Word\n";
    folderListMenu();    // pick a folder first
}

void MenuService::newWordInFolder(const Folder& f) {
    console.clearScreen();
    std::cout << "Adding word to \"" << f.getName() << "\"\n";

    std::string text, meaning, definition, pos;
    std::cout << "Word: ";
    std::getline(std::cin, text);
    std::cout << "Meaning: ";
    std::getline(std::cin, meaning);
    std::cout << "Definition: ";
    std::getline(std::cin, definition);
    std::cout << "Part of Speech: ";
    std::getline(std::cin, pos);

    // one example for simplicity
    std::cout << "Example sentence: ";
    std::string example;
    std::getline(std::cin, example);

    Word w(auth.getCurrentUserId(),
           text, meaning, definition, pos,
           f.getId());
    w.addExample(example);

    auto all = fm.loadWords();
    all.push_back(w);
    fm.saveWords(all);

    std::cout << "Word added!\n";
    console.waitForKey();
}

// ——— Simple Quiz ——————————————————————————————————————————————————

void MenuService::quizMenu(const Folder& f) {
    console.clearScreen();
    std::cout << "▶️  Quiz: " << f.getName() << "\n";

    // load words in this folder
    auto allWords = fm.loadWords();
    List<Word> quizWords;
    for (auto it = allWords.begin(); it != allWords.end(); ++it) {
        if ( (*it).getFolderId() == f.getId() ) {
            quizWords.push_back(*it);
        }
    }

    if (quizWords.begin() == quizWords.end()) {
        std::cout << "No words in this folder yet.\n";
        console.waitForKey();
        return;
    }

    int correct = 0, wrong = 0;
    auto start = std::chrono::steady_clock::now();

    // simple recall quiz
    for (int i = 0; i < quizWords.size(); ++i) {
        console.clearScreen();
        const Word& w = quizWords[i];
        std::cout << "Word [" << (i+1) << "/"
                  << quizWords.size() << "]: "
                  << w.getText() << "\n";
        std::cout << "Your answer: ";
        std::string ans;
        std::getline(std::cin, ans);
        if (ans == w.getMeaning()) {
            ++correct;
            std::cout << "✅ Correct!\n";
        } else {
            ++wrong;
            std::cout << "❌ Wrong. Meaning is: "
                      << w.getMeaning() << "\n";
        }
        console.waitForKey();
    }

    auto end = std::chrono::steady_clock::now();
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "\nQuiz Complete! "
              << correct << "/" << quizWords.size()
              << " correct in " << seconds << "s\n";

    // save the quiz
    Quiz q(auth.getCurrentUserId(),
           f.getId(),
           quizWords.size());
    q.recordResult(correct, wrong, seconds);
    auto all = fm.loadQuizzes();
    all.push_back(q);
    fm.saveQuizzes(all);

    std::cout << "Quiz saved.\n";
    console.waitForKey();
}
