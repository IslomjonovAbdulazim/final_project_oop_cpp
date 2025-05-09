#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>

class Question {
private:
    int         id;
    int         quizId;
    std::string prompt;
    std::string answer;

    static int  nextId;

public:
    Question(int quizId_, const std::string& prompt_, const std::string& ans_);
    Question(int id_, int quizId_, const std::string& prompt_, const std::string& ans_);

    int getId() const;
    int getQuizId() const;
    const std::string& getPrompt() const;
    bool checkAnswer(const std::string& ans) const;

    // CSV I/O
    std::string toCsv() const;
    static Question fromCsv(const std::string& line);
};

#endif // QUESTION_HPP
