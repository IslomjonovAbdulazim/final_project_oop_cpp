#include "Quiz.hpp"
#include <iostream>
#include <ctime>

int Quiz::nextId = 1;

// Default constructor
Quiz::Quiz()
  : id(nextId++)
  , userId(-1)
  , folderId(0)
  , totalQuestions(0)
  , correctCount(0)
  , wrongCount(0)
  , quizStart(std::time(nullptr))
  , quizEnd(quizStart)
{}

// Main constructor
Quiz::Quiz(int uid, int fId, int tQ)
  : id(nextId++)
  , userId(uid)
  , folderId(fId)
  , totalQuestions(tQ < 0 ? 0 : tQ)
  , correctCount(0)
  , wrongCount(0)
  , quizStart(std::time(nullptr))
  , quizEnd(quizStart)
{}

// Getters
int Quiz::getId() const              { return id; }
int Quiz::getUserId() const          { return userId; }
int Quiz::getFolderId() const        { return folderId; }
int Quiz::getTotalQuestions() const  { return totalQuestions; }
int Quiz::getCorrectCount() const    { return correctCount; }
int Quiz::getWrongCount() const      { return wrongCount; }
int Quiz::getTimeTaken() const       { return static_cast<int>(quizEnd - quizStart); }
std::time_t Quiz::getQuizDate() const{ return quizStart; }
List<int> Quiz::getQuestionIds() const { return questionIds; }

// Modifiers
void Quiz::addQuestion(int qid) {
    questionIds.push_back(qid);
}

void Quiz::recordResult(int correct, int wrong, int secondsTaken) {
    if (correct < 0) correct = 0;
    if (wrong   < 0) wrong   = 0;
    correctCount = correct;
    wrongCount   = wrong;
    int sum      = correct + wrong;
    if (sum > 0 && sum != totalQuestions)
        totalQuestions = sum;
    quizEnd = quizStart + secondsTaken;
}

// Print
void Quiz::printInfo() const {
    std::cout << "\n======== Quiz ========\n";
    std::cout << "ID:             " << id << "\n";
    std::cout << "User ID:        " << userId << "\n";
    std::cout << "Folder ID:      " << folderId << "\n";
    std::cout << "Questions:      " << totalQuestions << "\n";
    std::cout << "Correct:        " << correctCount << "\n";
    std::cout << "Wrong:          " << wrongCount << "\n";
    std::cout << "Time Taken:     " << getTimeTaken() << " s\n";
    std::cout << "Started:        " << std::ctime(&quizStart);
    std::cout << "Finished:       " << std::ctime(&quizEnd);
    std::cout << "Q IDs:          ";
    for (auto it = questionIds.begin(); it != questionIds.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n======================\n";
}
