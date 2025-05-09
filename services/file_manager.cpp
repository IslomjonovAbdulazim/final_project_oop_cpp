// src/services/file_manager.cpp

#include "file_manager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// ─── Helpers: split / join ────────────────────────────────────────────────────

List<std::string> FileManager::split(const std::string& line, char delim) {
    List<std::string> out;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, delim)) {
        out.push_back(item);
    }
    return out;
}

std::string FileManager::join(const List<std::string>& parts, char delim) {
    std::string out;
    bool first = true;
    for (auto it = parts.begin(); it != parts.end(); ++it) {
        if (!first) out += delim;
        first = false;
        out += *it;
    }
    return out;
}

// ─── Helper: safe indexing into a List<std::string> ─────────────────────────

static std::string getAt(const List<std::string>& lst, std::size_t idx) {
    auto it = lst.begin();
    for (std::size_t i = 0; i < idx && it != lst.end(); ++i) {
        ++it;
    }
    return (it != lst.end() ? *it : std::string{});
}

// ─── Constructor ──────────────────────────────────────────────────────────────

FileManager::FileManager(const std::string& basePath)
  : m_base(basePath)
{}

// ─── Folder CRUD ──────────────────────────────────────────────────────────────

List<Folder> FileManager::loadFolders() const {
    List<Folder> list;
    std::ifstream in(folderFile());
    if (!in) return list;

    std::string line;
    std::getline(in, line);  // skip header

    while (std::getline(in, line)) {
        auto cols = split(line, ',');
        // we expect at least  nine columns
        if (cols.size() < 9) continue;

        Folder f;
        f.setUserId(std::stoi(getAt(cols,1)));
        f.setName(getAt(cols,2));
        f.setDescription(getAt(cols,3));
        f.setWordsCount(std::stoi(getAt(cols,4)));
        // skip createdAt at idx=5
        f.setLastReviewedAt(
          static_cast<std::time_t>(std::stoll(getAt(cols,6)))
        );
        // quizzes at idx=8
        auto qs = split(getAt(cols,8), ';');
        for (auto qit = qs.begin(); qit != qs.end(); ++qit) {
            f.addQuizScore(std::stoi(*qit));
        }

        list.push_back(f);
    }
    return list;
}

void FileManager::saveFolders(const List<Folder>& folders) const {
    std::ofstream out(folderFile());
    out << "id,userId,name,description,wordsCount,"
           "createdAt,lastReviewedAt,lastUpdatedAt,"
           "quizzes,averageQuizScore\n";

    for (auto it = folders.begin(); it != folders.end(); ++it) {
        const Folder& f = *it;
        // gather quiz scores
        List<std::string> qs;
        for (auto qit = f.getQuizzes().begin(); qit != f.getQuizzes().end(); ++qit) {
            qs.push_back(std::to_string(*qit));
        }

        out
          << f.getId()                    << ","
          << f.getUserId()                << ","
          << f.getName()                  << ","
          << f.getDescription()           << ","
          << f.getWordsCount()            << ","
          << static_cast<long long>(f.getCreatedAt())       << ","
          << static_cast<long long>(f.getLastReviewedAt())  << ","
          << static_cast<long long>(f.getLastUpdatedAt())   << ","
          << join(qs, ';')                << ","
          << f.getAverageQuizScore()      << "\n";
    }
}

void FileManager::updateFolder(const Folder& folder) const {
    auto all     = loadFolders();
    List<Folder> updated;
    for (auto it = all.begin(); it != all.end(); ++it) {
        const Folder& f = *it;
        if (f.getId() == folder.getId()) {
            updated.push_back(folder);
        } else {
            updated.push_back(f);
        }
    }
    saveFolders(updated);
}

void FileManager::deleteFolder(int folderId) const {
    auto all     = loadFolders();
    List<Folder> filtered;
    for (auto it = all.begin(); it != all.end(); ++it) {
        const Folder& f = *it;
        if (f.getId() != folderId) {
            filtered.push_back(f);
        }
    }
    saveFolders(filtered);
}

// ─── Word CRUD ────────────────────────────────────────────────────────────────

List<Word> FileManager::loadWords() const {
    List<Word> list;
    std::ifstream in(wordFile());
    if (!in) return list;

    std::string line;
    std::getline(in, line);  // skip header

    while (std::getline(in, line)) {
        auto cols = split(line, ',');
        if (cols.size() < 13) continue;

        Word w;
        w.setUserId(std::stoi(getAt(cols,1)));
        w.setText(getAt(cols,2));
        w.setMeaning(getAt(cols,3));
        w.setDefinition(getAt(cols,4));
        w.setPartOfSpeech(getAt(cols,5));

        // examples at idx=6
        auto ex = split(getAt(cols,6), '|');
        for (auto eit = ex.begin(); eit != ex.end(); ++eit) {
            w.addExample(*eit);
        }

        w.setFolderId(std::stoi(getAt(cols,7)));
        // (timestamps and review counts can be handled similarly if you add setters)

        list.push_back(w);
    }
    return list;
}

void FileManager::saveWords(const List<Word>& words) const {
    std::ofstream out(wordFile());
    out << "id,userId,text,meaning,definition,partOfSpeech,"
           "examples,folderId,createdAt,lastReviewedAt,"
           "timesReviewed,correctAnswers,wrongAnswers\n";

    for (auto it = words.begin(); it != words.end(); ++it) {
        const Word& w = *it;
        List<std::string> exs;
        for (auto eit = w.getExamples().begin(); eit != w.getExamples().end(); ++eit) {
            exs.push_back(*eit);
        }

        out
          << w.getId()                 << ","
          << w.getUserId()             << ","
          << w.getText()               << ","
          << w.getMeaning()            << ","
          << w.getDefinition()         << ","
          << w.getPartOfSpeech()       << ","
          << join(exs, '|')            << ","
          << w.getFolderId()           << ","
          << static_cast<long long>(w.getCreatedAt())      << ","
          << static_cast<long long>(w.getLastReviewedAt()) << ","
          << w.getTimesReviewed()      << ","
          << w.getCorrectAnswers()     << ","
          << w.getWrongAnswers()       << "\n";
    }
}

// ─── Quiz CRUD ────────────────────────────────────────────────────────────────

List<Quiz> FileManager::loadQuizzes() const {
    List<Quiz> list;
    std::ifstream in(quizFile());
    if (!in) return list;

    std::string line;
    std::getline(in, line);  // skip header

    while (std::getline(in, line)) {
        auto cols = split(line, ',');
        if (cols.size() < 9) continue;

        Quiz q;
        q.setUserId(std::stoi(getAt(cols,1)));
        q.setFolderId(std::stoi(getAt(cols,2)));
        q.setTotalQuestions(std::stoi(getAt(cols,3)));
        q.recordResult(
          std::stoi(getAt(cols,4)),  // correct
          std::stoi(getAt(cols,5)),  // wrong
          std::stoi(getAt(cols,7)) - std::stoi(getAt(cols,6))  // duration
        );

        // question IDs at idx=8
        auto qs = split(getAt(cols,8), ';');
        for (auto qit = qs.begin(); qit != qs.end(); ++qit) {
            q.addQuestion(std::stoi(*qit));
        }

        list.push_back(q);
    }
    return list;
}

void FileManager::saveQuizzes(const List<Quiz>& quizzes) const {
    std::ofstream out(quizFile());
    out << "id,userId,folderId,totalQuestions,correctCount,wrongCount,"
           "quizStart,quizEnd,questionIds\n";

    for (auto it = quizzes.begin(); it != quizzes.end(); ++it) {
        const Quiz& q = *it;
        List<std::string> qs;
        for (auto qit = q.getQuestionIds().begin(); qit != q.getQuestionIds().end(); ++qit) {
            qs.push_back(std::to_string(*qit));
        }

        out
          << q.getId()                 << ","
          << q.getUserId()             << ","
          << q.getFolderId()           << ","
          << q.getTotalQuestions()     << ","
          << q.getCorrectCount()       << ","
          << q.getWrongCount()         << ","
          << static_cast<long long>(q.getQuizDate())              << ","
          << static_cast<long long>(q.getQuizDate() + q.getTimeTaken()) << ","
          << join(qs, ';')
          << "\n";
    }
}

// ─── Question CRUD ────────────────────────────────────────────────────────────

// inside src/services/file_manager.cpp:

List<Question> FileManager::loadQuestions() const {
    List<Question> list;
    std::ifstream in(questionFile());
    if (!in) return list;

    std::string line;
    std::getline(in, line);  // skip header

    while (std::getline(in, line)) {
        auto cols = split(line, ',');
        // we expect at least 9 columns:
        // 0:id, 1:userId, 2:wordId, 3:word, 4:options, 5:correctAnswer,
        // 6:userSelected, 7:folderId, 8:quizId
        if (cols.size() < 9) continue;

        // parse the semicolon-separated options
        auto opts = split(getAt(cols, 4), ';');

        // call your existing 6-arg ctor:
        int   userId        = std::stoi(getAt(cols, 1));
        int   wordId        = std::stoi(getAt(cols, 2));
        std::string text    = getAt(cols, 3);
        auto  optsList      = split(getAt(cols, 4), ';');
        std::string correct = getAt(cols, 5);
        std::string selected = getAt(cols, 6);              // might be empty
        int   folderId      = std::stoi(getAt(cols, 7));
        int   quizId        = std::stoi(getAt(cols, 8));

        // Now call the 7-arg ctor:
        Question q(
            userId,
            wordId,
            text,
            optsList,
            correct,
            folderId,
            quizId
        );

        // now set the CSV’s userId on it
        q.setUserId(std::stoi(getAt(cols, 1)));

        // if they’d previously selected an answer, restore it
        auto sel = getAt(cols, 6);
        if (!sel.empty()) {
            q.setUserSelected(sel);
        }

        list.push_back(q);
    }
    return list;
}


void FileManager::saveQuestions(const List<Question>& questions) const {
    std::ofstream out(questionFile());
    out << "id,userId,wordId,word,options,correctAnswer,"
           "userSelected,folderId,quizId\n";

    for (auto it = questions.begin(); it != questions.end(); ++it) {
        const Question& q = *it;
        List<std::string> opts;
        for (auto oit = q.getOptions().begin(); oit != q.getOptions().end(); ++oit) {
            opts.push_back(*oit);
        }

        out
          << q.getId()                << ","
          << q.getUserId()            << ","
          << q.getWordId()            << ","
          << q.getWord()              << ","
          << join(opts, ';')          << ","
          << q.getCorrectAnswer()     << ","
          << q.getUserSelected()      << ","
          << q.getFolderId()          << ","
          << q.getQuizId()
          << "\n";
    }
}
