#ifndef QUIZ_HPP
#define QUIZ_HPP

#include <ctime>
#include "../include/list.hpp"

class Quiz {
private:
    static int    nextId;
    int           id;
    int           userId;        // who took/created this quiz
    int           folderId;
    int           totalQuestions;
    int           correctCount;
    int           wrongCount;
    List<int>     questionIds;
    std::time_t   quizStart;
    std::time_t   quizEnd;

public:
    // Constructors
    Quiz();
    Quiz(int userId, int folderId, int totalQuestions);

    // Getters
    int getId() const;
    int getUserId() const;
    int getFolderId() const;
    int getTotalQuestions() const;
    int getCorrectCount() const;
    int getWrongCount() const;
    int getTimeTaken() const;      // seconds
    std::time_t getQuizDate() const;
    List<int> getQuestionIds() const;

    // Modifiers
    void addQuestion(int qid);
    void recordResult(int correct, int wrong, int secondsTaken);

    // Print
    void printInfo() const;

    void setUserId(int uid)             noexcept { userId = uid; }
    void setFolderId(int fid)           noexcept { folderId = fid; }
    void setTotalQuestions(int t)       noexcept { totalQuestions = t; }
};

#endif // QUIZ_HPP
