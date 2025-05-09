#ifndef QUIZ_HPP
#define QUIZ_HPP

#include <string>
#include <ctime>
#include "question.hpp"
#include "../include/list.hpp"

class Quiz {
private:
    int           id;
    int           folderId;
    std::time_t   startedAt;
    std::time_t   finishedAt;
    List<int>     questionIds;
    List<bool>    results;    // true = correct

    static int    nextId;

public:
    Quiz(int folderId_);
    Quiz(int id_, int folderId_, std::time_t sa, std::time_t fa,
         const List<int>& qids, const List<bool>& res);

    int           getId() const;
    int           getFolderId() const;
    void          addQuestion(int qid);
    void          recordResult(bool correct);
    void          finish();

    int           score() const;    // number correct
    int           total() const;    // number asked

    // CSV I/O
    std::string   toCsv() const;
    static Quiz   fromCsv(const std::string& line);
    std::time_t getStartedAt()  const;
    std::time_t getFinishedAt() const;

};

#endif // QUIZ_HPP
