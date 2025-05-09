#include "word.hpp"
#include <sstream>
#include <stdexcept>

int Word::nextId = 1;

static std::time_t parseTimeW(const std::string& s) {
    return static_cast<std::time_t>(std::stoll(s));
}

Word::Word(int folderId_, const std::string& term_, const std::string& def_)
    : id(nextId++), folderId(folderId_), term(term_), definition(def_),
      timesReviewed(0), avgScore(0.0), lastReviewedAt(0) {}

Word::Word(int id_, int folderId_, const std::string& term_, const std::string& def_,
           int tr, double av, std::time_t lr)
    : id(id_), folderId(folderId_), term(term_), definition(def_),
      timesReviewed(tr), avgScore(av), lastReviewedAt(lr) {
    if (id_ >= nextId) nextId = id_ + 1;
}

int Word::getId() const        { return id; }
int Word::getFolderId() const  { return folderId; }
const std::string& Word::getTerm() const { return term; }

void Word::updateReview(int score) {
    avgScore = ((avgScore * timesReviewed) + score) / double(timesReviewed + 1);
    timesReviewed++;
    lastReviewedAt = std::time(nullptr);
}

std::string Word::toCsv() const {
    std::ostringstream ss;
    ss << id << ',' << folderId << ',' << term << ',' << definition
       << ',' << timesReviewed << ',' << avgScore << ',' << lastReviewedAt;
    return ss.str();
}

Word Word::fromCsv(const std::string& line) {
    std::istringstream ss(line);
    std::string tok;
    int id_, fid, tr;
    double av;
    std::string term_, def_;
    std::time_t lr;

    std::getline(ss, tok, ','); id_ = std::stoi(tok);
    std::getline(ss, tok, ','); fid = std::stoi(tok);
    std::getline(ss, term_, ',');
    std::getline(ss, def_, ',');
    std::getline(ss, tok, ','); tr = std::stoi(tok);
    std::getline(ss, tok, ','); av = std::stod(tok);
    std::getline(ss, tok, ','); lr = parseTimeW(tok);

    if (id_ >= nextId) nextId = id_ + 1;
    return Word(id_, fid, term_, def_, tr, av, lr);
}
const std::string& Word::getDefinition() const {
    return definition;
}

double Word::getAvgScore() const {
    return avgScore;
}

std::time_t Word::getLastReviewedAt() const {
    return lastReviewedAt;
}
