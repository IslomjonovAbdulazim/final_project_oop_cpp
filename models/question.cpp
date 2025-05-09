#include "question.hpp"
#include <sstream>
#include <stdexcept>

int Question::nextId = 1;

Question::Question(int quizId_, const std::string& p, const std::string& a)
    : id(nextId++), quizId(quizId_), prompt(p), answer(a) {}

Question::Question(int id_, int quizId_, const std::string& p, const std::string& a)
    : id(id_), quizId(quizId_), prompt(p), answer(a) {
    if (id_ >= nextId) nextId = id_ + 1;
}

int Question::getId() const    { return id; }
int Question::getQuizId() const { return quizId; }
const std::string& Question::getPrompt() const { return prompt; }
bool Question::checkAnswer(const std::string& ans) const { return ans == answer; }

std::string Question::toCsv() const {
    std::ostringstream ss;
    ss << id << ',' << quizId << ',' << prompt << ',' << answer;
    return ss.str();
}

Question Question::fromCsv(const std::string& line) {
    std::istringstream ss(line);
    std::string tok, p, a;
    int id_, qid;

    std::getline(ss, tok, ','); id_ = std::stoi(tok);
    std::getline(ss, tok, ','); qid = std::stoi(tok);
    std::getline(ss, p, ',');
    std::getline(ss, a, ',');

    if (id_ >= nextId) nextId = id_ + 1;
    return Question(id_, qid, p, a);
}
