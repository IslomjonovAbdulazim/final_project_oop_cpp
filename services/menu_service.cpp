// services/menu_service.cpp
#include "menu_service.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <cctype>


static std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    auto end   = s.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}
static std::string toLower(const std::string& s) {
    std::string lc = s;
    std::transform(lc.begin(), lc.end(), lc.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return lc;
}
static std::string ltrim(const std::string& s) {
    auto p = s.find_first_not_of(" \t\n\r");
    if (p == std::string::npos) return "";
    return s.substr(p);
}
// Constructor
MenuService::MenuService(ConsoleService& cs, FileManager& fileMgr)
    : console(cs), fm(fileMgr), auth(fileMgr)
{}

// Entry point
void MenuService::run() {
    welcome();
    mainMenu();
}

// Welcome / login flow
void MenuService::welcome() {
    while (!currentUser) {
        std::cout << " Login or Signup\n\n";
        std::cout << "1) Login\n2) Signup\nChoice: ";
        int c = console.promptInt("", 1, 2);
        std::string user = console.prompt("Username: ");
        std::string pass = console.prompt("Password: ");
        user = trim(toLower(user));
        pass = trim(toLower(pass));
        try {
            if (c == 1) {
                currentUser = auth.login(user, pass);
                if (!currentUser) std::cout << " Invalid credentials.\n";
            } else {
                auth.signup(user, pass);
                auth.persist();
                currentUser = auth.login(user, pass);
                std::cout << " Account created.\n";
            }
        } catch (const std::exception& e) {
            std::cout << " Error: " << e.what() << "\n";
        }
    }
}

// Main menu
void MenuService::mainMenu() {
    while (true) {
        console.spacer();
        std::cout << "Main Menu\n\n";
        std::cout << "1) Manage Folders\n";
        std::cout << "2) Take Quiz\n\n";
        std::cout << "[F]olders  [Q]uiz  [E]xit\n\n";
        std::string inp = console.prompt("Enter choice: ");
        if (inp.empty()) continue;
        char cmd = std::toupper(inp.at(0));
        if (std::isdigit(cmd)) {
            int n = std::stoi(inp);
            if (n == 1) manageFolders();
            else if (n == 2) takeQuiz();
            else std::cout << "Invalid number.\n";
        } else {
            switch (cmd) {
            case 'F': manageFolders(); break;
            case 'Q': takeQuiz(); break;
            case 'E': return;
            default: std::cout << "Unknown command.\n";
            }
        }
        console.pause();
    }
}

// Folder CRUD & stats

void MenuService::manageFolders() {
    int userId = currentUser->getId();
    while (true) {
        console.spacer();

        auto folders = fm.loadFolders();
        List<Folder> filtered;

        for (auto it = folders.begin(); it != folders.end(); ++it) {
            if ((*it).getUserId() == userId) {
                filtered.add(std::move(*it));  // move instead of copy
            }
        }
        folders = std::move(filtered);
        List<Folder*> view;
        for (std::size_t i = 0; i < folders.size(); ++i)
            view.add(&folders.get(i));

        // Bubble sort (case-insensitive) on our custom List
        auto toLower = [](const std::string& s) {
            std::string result = s;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            return result;
        };
        for (std::size_t i = 0; i < view.size(); ++i) {
            for (std::size_t j = 0; j + 1 < view.size() - i; ++j) {
                Folder*& a = view.get(j);
                Folder*& b = view.get(j + 1);
                if (toLower(a->getName()) > toLower(b->getName())) {
                    std::swap(a, b);
                }
            }
        }

        // 2) Display
        std::cout << " Your Folders & Stats \n\n";
        for (std::size_t i = 0; i < view.size(); ++i) {
            const auto& f = *view.get(i);
            std::printf(
                "%2zu)  %-15s  %3d  %3zu  %5.1f%%\n",
                i + 1,
                f.getName().c_str(),
                f.getWordsCount(),
                f.getQuizCount(),
                f.averageScore()
            );
        }

        std::cout << "\n"
                  << "[C] Create  [D] Delete  [R] Rename  "
                  << "[W] Words  [V] Results  [B] Back\n\n";

        // 3) Read input
        std::string raw = console.prompt(" Folder # or cmd: ");
        std::string inp = ltrim(raw);
        if (inp.empty()) continue;
        char cmd = std::toupper(static_cast<unsigned char>(inp.at(0)));

        // 4) Numeric selection → open words
        if (std::isdigit(cmd)) {
            int sel = std::stoi(inp) - 1;
            if (sel >= 0 && sel < int(view.size())) {
                manageWords(view.get(sel)->getId());
                continue;
            }
        }

        // 5) Letter commands
        switch (cmd) {
            case 'C': {
                    std::string name = console.prompt("New folder name: ");
                    std::string desc = console.prompt("Description: ");

                    // Load full folder list
                    auto allFolders = fm.loadFolders();
                    allFolders.add(Folder(currentUser->getId(), name, desc));
                    fm.saveFolders(allFolders);
                    break;
            }
            case 'D': {
                    int num = console.promptInt("Delete which #? ", 1, int(view.size()));
                    int delIndex = num - 1;
                    if (delIndex >= 0 && delIndex < int(view.size())) {
                        int delId = view.get(delIndex)->getId();

                        // Load full folder list
                        auto allFolders = fm.loadFolders();
                        List<Folder> keep;
                        for (auto it = allFolders.begin(); it != allFolders.end(); ++it) {
                            if ((*it).getId() != delId) {
                                keep.add(std::move(*it));
                            }
                        }
                        fm.saveFolders(keep);
                    } else {
                        std::cout << " Invalid folder number.\n";
                    }
                    break;
            }
            case 'R': {
                    int num = console.promptInt("Rename which #? ", 1, int(view.size()));
                    int renIndex = num - 1;
                    if (renIndex >= 0 && renIndex < int(view.size())) {
                        int renId = view.get(renIndex)->getId();
                        std::string newName = console.prompt("New name: ");
                        std::string newDesc = console.prompt("New desc: ");

                        // Load full folder list
                        auto allFolders = fm.loadFolders();
                        for (auto it = allFolders.begin(); it != allFolders.end(); ++it) {
                            if ((*it).getId() == renId) {
                                (*it).setName(newName);
                                (*it).setDescription(newDesc);
                                break;
                            }
                        }
                        fm.saveFolders(allFolders);
                    } else {
                        std::cout << " Invalid folder number.\n";
                    }
                    break;
            }
            case 'W': {
                int num = console.promptInt("Open words for folder #? ", 1, int(view.size()));
                int wIndex = num - 1;
                if (wIndex >= 0 && wIndex < int(view.size())) {
                    manageWords(view.get(wIndex)->getId());
                } else {
                    std::cout << " Invalid folder number.\n";
                }
                break;
            }
            case 'V': {
                int num = console.promptInt("View results for folder #? ", 1, int(view.size()));
                int vIndex = num - 1;
                if (vIndex >= 0 && vIndex < int(view.size())) {
                    manageQuizResults(view.get(vIndex)->getId());
                } else {
                    std::cout << " Invalid folder number.\n";
                }
                break;
            }
            case 'B':
                return;
            default:
                std::cout << " Unrecognized command.\n";
        }

        console.pause();
    }
}

void MenuService::manageQuizResults(int folderId) {
    console.spacer();

    auto allQuizzes = fm.loadQuizzes();
    // collect pointers to the matching quizzes
    std::vector<Quiz*> results;
    for (auto it = allQuizzes.begin(); it != allQuizzes.end(); ++it) {
        if ((*it).getFolderId() == folderId) {
            results.push_back(&(*it));
        }
    }

    if (results.empty()) {
        std::cout << " No quiz results for this folder yet.\n";
        console.pause();
        return;
    }

    // Display header
    std::cout << " Quiz Results\n\n";
    std::cout << " #  Date & Time           Score\n";
    std::cout << "-----------------------------------\n";

    for (std::size_t i = 0; i < results.size(); ++i) {
        Quiz* q = results.at(i);

        // format finishedAt
        std::time_t ft = q->getFinishedAt();
        char buf[20] = "--";
        if (ft > 0) {
            std::tm* tm = std::localtime(&ft);
            std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", tm);
        }

        int score = q->score();
        int total = q->total();
        std::printf(
            "%2zu) %-19s  %2d /%2d\n",
            i+1,
            buf,
            score,
            total
        );
    }

    console.pause();
}

void MenuService::manageWords(int folderId) {
    while (true) {
        console.spacer();

        // 1) Load & filter
        auto allWords = fm.loadWords();
        List<Word> words;
        for (auto it = allWords.begin(); it != allWords.end(); ++it) {
            if ((*it).getFolderId() == folderId) {
                words.add(*it);
            }
        }
        if (words.empty()) {
            std::cout << " No words in this folder.\n";
            console.pause();
            return;
        }

        // 2) Bubble-sort by term (case-insensitive)
        auto icase = [](char c){ return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); };
        for (std::size_t i = 0; i < words.size(); ++i) {
            for (std::size_t j = 0; j + 1 < words.size() - i; ++j) {
                const auto& a = words.get(j).getTerm();
                const auto& b = words.get(j+1).getTerm();
                bool greater = false;
                for (std::size_t k = 0; k < a.size() && k < b.size(); ++k) {
                    char ca = icase(a.at(k)), cb = icase(b.at(k));
                    if (ca != cb) { greater = (ca > cb); break; }
                }
                if (!greater && a.size() > b.size()) greater = true;
                if (greater) std::swap(words.get(j), words.get(j+1));
            }
        }

        // 3) Display header with last‐reviewed column
        std::cout << " Words (A→Z) \n\n";
        std::printf("%3s  %-15s  %-25s  %7s  %-19s\n",
                    "#", "Term", "Definition", "% Known", "Last Reviewed");
        std::printf("--------------------------------------------------------------------------------\n");

        // 4) List each word with its percent known and last‐reviewed timestamp
        for (std::size_t i = 0; i < words.size(); ++i) {
            const auto& w = words.get(i);
            double pct = w.getAvgScore() * 100.0;  // 0–100

            // format lastReviewedAt
            std::string lrStr = "--";
            std::time_t lr = w.getLastReviewedAt();
            if (lr > 0) {
                std::tm* tm = std::localtime(&lr);
                char buf[20];
                std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
                lrStr = buf;
            }

            std::printf(
                "%3zu) %-15s  %-25s  %5.1f%%   %s\n",
                i+1,
                w.getTerm().c_str(),
                w.getDefinition().c_str(),
                pct,
                lrStr.c_str()
            );
        }

        // 5) Actions
        std::cout << "\n[A] Add   [D] Delete   [B] Back\n\n";
        std::string raw = console.prompt("  Word # or cmd: ");
        std::string inp = ltrim(raw);
        if (inp.empty()) continue;
        char cmd = std::toupper(static_cast<unsigned char>(inp.at(0)));

        if (std::isdigit(cmd)) {
            int n = std::stoi(inp) - 1;
            if (n >= 0 && n < int(words.size())) {
                std::cout << " Reviewing \"" << words.get(n).getTerm() << "\"...\n";
            } else {
                std::cout << " Invalid word number.\n";
            }
        } else {
            switch (cmd) {
                case 'A': {
                    std::string term = console.prompt("Term: ");
                    std::string def  = console.prompt("Definition: ");
                    allWords.add(Word(folderId, term, def));
                    fm.saveWords(allWords);
                    // bump folder count...
                    break;
                }
                case 'D': {
                    int d = console.promptInt("Delete which #? ", 1, int(words.size())) - 1;
                    if (d >= 0 && d < int(words.size())) {
                        List<Word> keep;
                        for (auto it = allWords.begin(); it != allWords.end(); ++it) {
                            if (!((*it).getFolderId() == folderId &&
                                  (*it).getId()       == words.get(d).getId()))
                                keep.add(*it);
                        }
                        fm.saveWords(keep);
                    } else {
                        std::cout << " Invalid number.\n";
                    }
                    break;
                }
                case 'B':
                    return;
                default:
                    std::cout << " Unknown command.\n";
            }
        }

        console.pause();
    }
}


// Quiz flow
void MenuService::takeQuiz() {
    console.spacer();

    // 1) Pick a folder
    auto folders = fm.loadFolders();
    if (folders.empty()) {
        std::cout << "⚠️ No folders to quiz.\n";
        console.pause();
        return;
    }
    std::cout << "🧪  Select folder to quiz:\n";
    for (size_t i = 0; i < folders.size(); ++i)
        std::cout << (i+1) << ") " << folders.get(i).getName() << "\n";
    int idx = console.promptInt("Choice: ", 1, (int)folders.size()) - 1;
    int folderId = folders.get(idx).getId();

    // 2) Build pool
    auto allWords = fm.loadWords();
    std::vector<Word*> pool;
    for (auto it = allWords.begin(); it != allWords.end(); ++it)
        if ((*it).getFolderId() == folderId)
            pool.push_back(&(*it));
    if (pool.empty()) {
        std::cout << "⚠️ Folder has no words.\n";
        console.pause();
        return;
    }
    std::shuffle(pool.begin(), pool.end(), std::mt19937{std::random_device{}()});
    size_t quizN = std::min(pool.size(), size_t(10));

    // 3) Run quiz
    Quiz quiz(folderId);
    for (size_t q = 0; q < quizN; ++q) {
        Word* w = pool.at(q);
        std::cout << "❓ Q" << (q+1) << ": " << w->getDefinition() << "\n> ";
        std::string rawAns = console.prompt("");
        // Normalize both strings
        std::string userAns    = toLower(trim(rawAns));
        std::string correctAns = toLower(trim(w->getTerm()));
        bool correct = (userAns == correctAns);
        quiz.addQuestion(w->getId());
        quiz.recordResult(correct);
        std::cout << (correct
                       ? "✅ Correct!\n"
                       : "❌ Wrong! Ans: " + w->getTerm() + "\n");
        w->updateReview(correct ? 1 : 0);
        console.pause();
    }
    quiz.finish();

    // 4) Show score
    int score = quiz.score();
    int total = quiz.total();  // capture while quiz is still valid
    std::cout << "🏁 Quiz Complete: " << score << " / " << total << "\n";
    console.pause();

    // 5) Persist quiz
    auto quizzes = fm.loadQuizzes();
    quizzes.add(std::move(quiz));
    fm.saveQuizzes(quizzes);

    // 6) Update folder stats
    auto fs = fm.loadFolders();
    for (size_t i = 0; i < fs.size(); ++i) {
        if (fs.get(i).getId() == folderId) {
            fs.get(i).addQuizResult(score, total);  // use captured total
            break;
        }
    }
    fm.saveFolders(fs);

    // 7) Persist word stats
    fm.saveWords(allWords);
}

