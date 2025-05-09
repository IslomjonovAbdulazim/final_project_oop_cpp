#include "quiz.hpp"
#include <sstream>
#include <stdexcept>

int Quiz::nextId = 1;

static std::time_t parseTimeQ(const std::string& s) {
    return static_cast<std::time_t>(std::stoll(s));
}

Quiz::Quiz(int folderId_)
    : id(nextId++), folderId(folderId_),
      startedAt(std::time(nullptr)), finishedAt(0)
{
}

Quiz::Quiz(int id_, int fid, std::time_t sa, std::time_t fa,
           const List<int>& qids, const List<bool>& res)
    : id(id_), folderId(fid), startedAt(sa), finishedAt(fa)
{
    if (id_ >= nextId) nextId = id_ + 1;
    // copy question IDs
    for (auto it = qids.begin(); it != qids.end(); ++it) {
        questionIds.add(*it);
    }
    // copy results
    for (auto it = res.begin(); it != res.end(); ++it) {
        results.add(*it);
    }
}

int Quiz::getId() const          { return id; }
int Quiz::getFolderId() const    { return folderId; }

void Quiz::addQuestion(int qid)  { questionIds.add(qid); }
void Quiz::recordResult(bool correct) { results.add(correct); }

void Quiz::finish() {
    finishedAt = std::time(nullptr);
}

int Quiz::score() const {
    int s = 0;
    for (auto it = results.begin(); it != results.end(); ++it)
        if (*it) ++s;
    return s;
}

int Quiz::total() const {
    return static_cast<int>(results.size());
}

std::string Quiz::toCsv() const {
    std::ostringstream ss;
    ss << id << ',' << folderId << ',' << startedAt << ',' << finishedAt
       << ',' << questionIds.size();
    for (std::size_t i = 0; i < questionIds.size(); ++i)
        ss << ',' << questionIds.get(i);
    ss << ',' << results.size();
    for (std::size_t i = 0; i < results.size(); ++i)
        ss << ',' << (results.get(i) ? 1 : 0);
    return ss.str();
}

Quiz Quiz::fromCsv(const std::string& line) {
    std::istringstream ss(line);
    std::string tok;
    int id_, fid, cntQ, cntR;
    std::time_t sa, fa;

    std::getline(ss, tok, ','); id_ = std::stoi(tok);
    std::getline(ss, tok, ','); fid = std::stoi(tok);
    std::getline(ss, tok, ','); sa = parseTimeQ(tok);
    std::getline(ss, tok, ','); fa = parseTimeQ(tok);
    std::getline(ss, tok, ','); cntQ = std::stoi(tok);

    List<int> qids;
    for (int i = 0; i < cntQ; ++i) {
        std::getline(ss, tok, ',');
        qids.add(std::stoi(tok));
    }

    std::getline(ss, tok, ','); cntR = std::stoi(tok);
    List<bool> res;
    for (int i = 0; i < cntR; ++i) {
        std::getline(ss, tok, ',');
        res.add(tok == "1");
    }

    return Quiz(id_, fid, sa, fa, qids, res);
}
std::time_t Quiz::getStartedAt() const {
    return startedAt;
}

std::time_t Quiz::getFinishedAt() const {
    return finishedAt;
}