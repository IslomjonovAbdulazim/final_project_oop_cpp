#include "Question.hpp"
#include <iostream>

int Question::nextId = 1;

// Default constructor
Question::Question()
  : id(nextId++)
  , userId(-1)
  , wordId(0)
  , word("")
  , options()
  , correctAnswer("")
  , userSelected("")
  , folderId(0)
  , quizId(0)
{}

// Main constructor
Question::Question(int uid,
                   int wId,
                   const std::string& w,
                   const List<std::string>& opts,
                   const std::string& corr,
                   int fId,
                   int qId)
  : id(nextId++)
  , userId(uid)
  , wordId(wId)
  , word(w)
  , options(opts)
  , correctAnswer(corr)
  , userSelected("")
  , folderId(fId)
  , quizId(qId)
{
    // ensure correctAnswer is in options
    bool found = false;
    for (auto it = options.begin(); it != options.end(); ++it) {
        if (*it == correctAnswer) { found = true; break; }
    }
    if (!found) {
        options.push_back(correctAnswer);
    }
}

// Getters
int Question::getId()               const { return id; }
int Question::getUserId()           const { return userId; }
int Question::getWordId()           const { return wordId; }
std::string Question::getWord()     const { return word; }
List<std::string> Question::getOptions() const { return options; }
std::string Question::getCorrectAnswer() const { return correctAnswer; }
std::string Question::getUserSelected()  const { return userSelected; }
int Question::getFolderId()         const { return folderId; }
int Question::getQuizId()           const { return quizId; }


// Check correctness
bool Question::isCorrect() const {
    return (!userSelected.empty() && userSelected == correctAnswer);
}

// Print
void Question::printQuestion() const {
    std::cout << "\n---- Question " << id << " ----\n";
    std::cout << word << "\n";
    char label = 'A';
    for (auto it = options.begin(); it != options.end(); ++it) {
        std::cout << "  " << label++ << ") " << *it << "\n";
    }
    if (!userSelected.empty()) {
        std::cout << "\nYou chose: " << userSelected
                  << (isCorrect() ? "  [✓]" : "  [✗]") << "\n";
    }
    std::cout << "-----------------------\n";
}
