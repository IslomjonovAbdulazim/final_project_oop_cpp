#include "Folder.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

int Folder::nextId = 1;

// Constructors
Folder::Folder()
    : id(nextId++)
    , userId(-1)
    , name("Untitled")
    , description("No Description")
    , wordsCount(0)
    , createdAt(std::time(nullptr))
    , lastReviewedAt(createdAt)
    , lastUpdatedAt(createdAt)
    , totalQuizScore(0.0)
    , averageQuizScore(0.0)
{}

Folder::Folder(int uid, const std::string& n, const std::string& d)
    : id(nextId++)
    , userId(uid)
    , name(n)
    , description(d)
    , wordsCount(0)
    , createdAt(std::time(nullptr))
    , lastReviewedAt(createdAt)
    , lastUpdatedAt(createdAt)
    , totalQuizScore(0.0)
    , averageQuizScore(0.0)
{}

// Getters
int Folder::getId()                 const { return id; }
int Folder::getUserId()             const { return userId; }
std::string Folder::getName()       const { return name; }
std::string Folder::getDescription()const { return description; }
int Folder::getWordsCount()         const { return wordsCount; }
std::time_t Folder::getCreatedAt()  const { return createdAt; }
std::time_t Folder::getLastReviewedAt() const { return lastReviewedAt; }
std::time_t Folder::getLastUpdatedAt()  const { return lastUpdatedAt; }
List<int> Folder::getQuizzes()      const { return quizzes; }
double Folder::getAverageQuizScore()const { return averageQuizScore; }

// Setters
void Folder::setUserId(int uid)               { userId = uid; lastUpdatedAt = std::time(nullptr); }
void Folder::setName(const std::string& n)    { name = n; lastUpdatedAt = std::time(nullptr); }
void Folder::setDescription(const std::string& d) { description = d; lastUpdatedAt = std::time(nullptr); }
void Folder::setWordsCount(int c)             { wordsCount = (c<0?0:c); lastUpdatedAt = std::time(nullptr); }
void Folder::setLastReviewedAt(std::time_t t) { lastReviewedAt = t; lastUpdatedAt = std::time(nullptr); }

// Quiz‐score methods
void Folder::addQuizScore(int score) {
    quizzes.push_back(score);
    totalQuizScore   += score;
    averageQuizScore  = quizzes.size() ? (totalQuizScore / quizzes.size()) : 0.0;
    lastUpdatedAt     = std::time(nullptr);
}

bool Folder::removeQuizScore(int score) {
    bool removed = false;
    List<int> kept;
    for (auto it = quizzes.begin(); it != quizzes.end(); ++it) {
        if (!removed && *it == score) {
            totalQuizScore -= score;
            removed = true;
        } else {
            kept.push_back(*it);
        }
    }
    if (removed) {
        quizzes.clear();
        for (auto it = kept.begin(); it != kept.end(); ++it) {
            quizzes.push_back(*it);
        }
        averageQuizScore = quizzes.size() ? (totalQuizScore / quizzes.size()) : 0.0;
        lastUpdatedAt    = std::time(nullptr);
    }
    return removed;
}

void Folder::clearQuizScores() {
    quizzes.clear();
    totalQuizScore   = 0.0;
    averageQuizScore = 0.0;
    lastUpdatedAt    = std::time(nullptr);
}

// Print
void Folder::printInfo() const {
    std::cout << "\n========== Folder ==========\n";
    std::cout << "ID:              " << id << "\n";
    std::cout << "Owner User ID:   " << userId << "\n";
    std::cout << "Name:            " << name << "\n";
    std::cout << "Description:     " << description << "\n";
    std::cout << "Words Count:     " << wordsCount << "\n";
    std::cout << "Quizzes Taken:   " << quizzes.size() << "\n";
    std::cout << "Average Score:   " << averageQuizScore << "%\n";
    std::cout << "Quiz Scores:     ";
    for (auto it = quizzes.begin(); it != quizzes.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";
    std::cout << "Created At:      " << std::ctime(&createdAt);
    std::cout << "Last Reviewed:   " << std::ctime(&lastReviewedAt);
    std::cout << "Last Updated:    " << std::ctime(&lastUpdatedAt);
    std::cout << "=============================\n\n";
}
