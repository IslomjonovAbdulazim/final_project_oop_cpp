#include "Word.hpp"
#include <iostream>
#include <ctime>

int Word::nextId = 1;

// Constructors
Word::Word()
  : id(nextId++)
  , userId(-1)
  , text("Untitled")
  , meaning("")
  , definition("")
  , partOfSpeech("")
  , folderId(-1)
  , createdAt(std::time(nullptr))
  , lastReviewedAt(createdAt)
  , timesReviewed(0)
  , correctAnswers(0)
  , wrongAnswers(0)
{}

Word::Word(int uid,
           const std::string& t,
           const std::string& m,
           const std::string& d,
           const std::string& pos,
           int fid)
  : id(nextId++)
  , userId(uid)
  , text(t)
  , meaning(m)
  , definition(d)
  , partOfSpeech(pos)
  , folderId(fid)
  , createdAt(std::time(nullptr))
  , lastReviewedAt(createdAt)
  , timesReviewed(0)
  , correctAnswers(0)
  , wrongAnswers(0)
{}

// Getters
int Word::getId()           const { return id; }
int Word::getUserId()       const { return userId; }
std::string Word::getText() const { return text; }
std::string Word::getMeaning() const { return meaning; }
std::string Word::getDefinition() const { return definition; }
std::string Word::getPartOfSpeech() const { return partOfSpeech; }
List<std::string> Word::getExamples() const { return examples; }
int Word::getFolderId()     const { return folderId; }
std::time_t Word::getCreatedAt()   const { return createdAt; }
std::time_t Word::getLastReviewedAt() const { return lastReviewedAt; }
int Word::getTimesReviewed() const { return timesReviewed; }
int Word::getCorrectAnswers() const { return correctAnswers; }
int Word::getWrongAnswers()   const { return wrongAnswers; }

// Setters
void Word::setUserId(int uid)                 { userId = uid; }
void Word::setMeaning(const std::string& m)   { meaning = m; }
void Word::setDefinition(const std::string& d){ definition = d; }
void Word::setPartOfSpeech(const std::string& p){ partOfSpeech = p; }
void Word::setFolderId(int fid)               { folderId = fid; }
void Word::addExample(const std::string& ex)  { examples.push_back(ex); }

// Review tracking
void Word::recordReview(bool correct) {
    ++timesReviewed;
    lastReviewedAt = std::time(nullptr);
    if (correct) ++correctAnswers;
    else         ++wrongAnswers;
}

// Print
void Word::printInfo() const {
    std::cout << "\n---------- Word ----------\n";
    std::cout << "ID:            " << id << "\n";
    std::cout << "Owner ID:      " << userId << "\n";
    std::cout << "Word:          " << text << "\n";
    std::cout << "Meaning:       " << meaning << "\n";
    std::cout << "Definition:    " << definition << "\n";
    std::cout << "Part of Speech:" << partOfSpeech << "\n";
    std::cout << "Folder ID:     " << folderId << "\n";
    std::cout << "Reviewed:      " << timesReviewed
              << " (+" << correctAnswers << "/-" << wrongAnswers << ")\n";
    std::cout << "Created At:    " << std::ctime(&createdAt);
    std::cout << "Last Reviewed: " << std::ctime(&lastReviewedAt);
    std::cout << "Examples:\n";
    for (auto it = examples.begin(); it != examples.end(); ++it)
        std::cout << "  - " << *it << "\n";
    std::cout << "--------------------------\n";
}
