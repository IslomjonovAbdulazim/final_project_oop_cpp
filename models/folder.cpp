#include "folder.hpp"
#include <sstream>

// Static init
int Folder::nextId = 1;

Folder::Folder(int userId_, const std::string& name_, const std::string& desc_)
    : id(nextId++),
      userId(userId_),
      name(name_),
      description(desc_),
      wordsCount(0),
      quizResults(),
      createdAt(std::time(nullptr)),
      lastReviewedAt(0),
      lastUpdatedAt(std::time(nullptr))
{}

Folder::Folder(int id_, int userId_,
               const std::string& name_, const std::string& desc_,
               int wordsCount_,
               List<std::pair<int,int>> results_,
               std::time_t c_, std::time_t lr_, std::time_t lu_)
    : id(id_),
      userId(userId_),
      name(name_),
      description(desc_),
      wordsCount(wordsCount_),
      quizResults(std::move(results_)),
      createdAt(c_),
      lastReviewedAt(lr_),
      lastUpdatedAt(lu_)
{
    if (id_ >= nextId) nextId = id_ + 1;
}

void Folder::addQuizResult(int correct, int total) {
    quizResults.add(std::pair<int,int>(correct, total));
    lastUpdatedAt = std::time(nullptr);
}

double Folder::averageScore() const {
    if (quizResults.empty()) return 0.0;
    double acc = 0.0;
    for (auto it = quizResults.begin(); it != quizResults.end(); ++it) {
        int correct = (*it).first;
        int total   = (*it).second;
        if (total > 0) acc += double(correct) / double(total);
    }
    return (acc / double(quizResults.size())) * 100.0;
}

std::string Folder::toCsv() const {
    std::ostringstream ss;
    ss << id << ','
       << userId << ','
       << name << ','
       << description << ','
       << wordsCount << ','
       << createdAt << ','
       << lastReviewedAt << ','
       << lastUpdatedAt << ','
       << quizResults.size();
    for (auto it = quizResults.begin(); it != quizResults.end(); ++it) {
        ss << ',' << (*it).first << '/' << (*it).second;
    }
    return ss.str();
}

Folder Folder::fromCsv(const std::string& line) {
    std::istringstream ss(line);
    std::string tok;
    int id_, uid, wc;
    std::string nm, desc;
    std::time_t c, lr, lu;

    std::getline(ss, tok, ','); id_ = std::stoi(tok);
    std::getline(ss, tok, ','); uid = std::stoi(tok);
    std::getline(ss, nm,  ',');
    std::getline(ss, desc, ',');
    std::getline(ss, tok, ','); wc = std::stoi(tok);
    std::getline(ss, tok, ','); c  = std::stoll(tok);
    std::getline(ss, tok, ','); lr = std::stoll(tok);
    std::getline(ss, tok, ','); lu = std::stoll(tok);
    std::getline(ss, tok, ','); int qc = std::stoi(tok);

    List<std::pair<int,int>> results;
    for (int i = 0; i < qc; ++i) {
        if (!std::getline(ss, tok, ',')) break;
        auto pos = tok.find('/');
        int correct = std::stoi(tok.substr(0, pos));
        int total   = std::stoi(tok.substr(pos + 1));
        results.add(std::pair<int,int>(correct, total));
    }

    return Folder(id_, uid, nm, desc, wc, std::move(results), c, lr, lu);
}
