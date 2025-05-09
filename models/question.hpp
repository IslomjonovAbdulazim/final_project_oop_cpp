#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>
#include "../include/list.hpp"

class Question {
private:
    static int          nextId;
    int                  id;
    int                  userId;        // who created it
    int                  wordId;
    std::string          word;
    List<std::string>    options;
    std::string          correctAnswer;
    std::string          userSelected;
    int                  folderId;
    int                  quizId;

public:
    // Constructors
    Question();
    Question(int userId,
             int wordId,
             const std::string& word,
             const List<std::string>& opts,
             const std::string& correctAnswer,
             int folderId,
             int quizId);

    // Getters
    int getId() const;
    int getUserId() const;
    int getWordId() const;
    std::string getWord() const;
    List<std::string> getOptions() const;
    std::string getCorrectAnswer() const;
    std::string getUserSelected() const;
    int getFolderId() const;
    int getQuizId() const;

    // Set user selection
    // Check correctness
    bool isCorrect() const;

    // Print
    void printQuestion() const;
    void setUserId(int uid) noexcept     { userId = uid; }
    void setUserSelected(const std::string& s) { userSelected = s; }
};

#endif // QUESTION_HPP
