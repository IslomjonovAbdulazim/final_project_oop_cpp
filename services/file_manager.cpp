#include "file_manager.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include <utility>

namespace fs = std::filesystem;

FileManager::FileManager(const std::string& dataDir)
    : baseDir(dataDir)
{
    fs::create_directories(baseDir);
    ensureCsv("users.csv",     "id,username,passwordHash,createdAt\n");
    ensureCsv("folders.csv",   "id,userId,name,description,wordsCount,createdAt,lastReviewedAt,lastUpdatedAt,quizCount,scores...\n");
    ensureCsv("words.csv",     "id,folderId,term,definition,timesReviewed,avgScore,lastReviewedAt\n");
    ensureCsv("quizzes.csv",   "id,folderId,startedAt,finishedAt,questionCount,questionIds...,resultCount,results...\n");
    ensureCsv("questions.csv", "id,quizId,prompt,answer\n");
}

void FileManager::ensureCsv(const std::string& fn, const std::string& hdr) {
    auto path = fs::path(baseDir) / fn;
    if (!fs::exists(path)) {
        std::ofstream out(path);
        out << hdr;
    }
}

template<typename T>
List<T> FileManager::loadAll(const std::string& filename, T(*fromCsv)(const std::string&)) {
    List<T> list;
    auto path = fs::path(baseDir) / filename;
    std::ifstream in(path);
    std::string line;
    bool first = true;
    while (std::getline(in, line)) {
        if (first) { first = false; continue; }
        if (line.empty()) continue;
        try {
            T obj = fromCsv(line);
            list.add(std::move(obj));
        } catch (const std::exception& e) {
            std::cerr << "[Warning] failed parsing " << filename << ": " << e.what() << "\n";
        }
    }
    return std::move(list);
}

template<typename T>
void FileManager::saveAll(const std::string& filename, const List<T>& items) {
    auto path = fs::path(baseDir) / filename;
    std::ofstream out(path);
    // rewrite header based on filename
    if (filename == "users.csv")
        out << "id,username,passwordHash,createdAt\n";
    else if (filename == "folders.csv")
        out << "id,userId,name,description,wordsCount,createdAt,lastReviewedAt,lastUpdatedAt,quizCount,scores...\n";
    else if (filename == "words.csv")
        out << "id,folderId,term,definition,timesReviewed,avgScore,lastReviewedAt\n";
    else if (filename == "quizzes.csv")
        out << "id,folderId,startedAt,finishedAt,questionCount,questionIds...,resultCount,results...\n";
    else if (filename == "questions.csv")
        out << "id,quizId,prompt,answer\n";

    for (auto it = items.begin(); it != items.end(); ++it) {
        out << (*it).toCsv() << '\n';
    }
}

List<User>     FileManager::loadUsers()     { return loadAll("users.csv", User::fromCsv); }
void           FileManager::saveUsers(const List<User>& u) { saveAll("users.csv", u); }

List<Folder>   FileManager::loadFolders()   { return loadAll("folders.csv", Folder::fromCsv); }
void           FileManager::saveFolders(const List<Folder>& f) { saveAll("folders.csv", f); }

List<Word>     FileManager::loadWords()     { return loadAll("words.csv", Word::fromCsv); }
void           FileManager::saveWords(const List<Word>& w) { saveAll("words.csv", w); }

List<Quiz>     FileManager::loadQuizzes()   { return loadAll("quizzes.csv", Quiz::fromCsv); }
void           FileManager::saveQuizzes(const List<Quiz>& q) { saveAll("quizzes.csv", q); }

List<Question> FileManager::loadQuestions(){ return loadAll("questions.csv", Question::fromCsv); }
void           FileManager::saveQuestions(const List<Question>& q) { saveAll("questions.csv", q); }
