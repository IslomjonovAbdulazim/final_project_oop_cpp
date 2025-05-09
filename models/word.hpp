#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <ctime>

class Word {
private:
    int         id;
    int         folderId;
    std::string term;
    std::string definition;

    int         timesReviewed;
    double      avgScore;
    std::time_t lastReviewedAt;

    static int  nextId;

public:
    Word(int folderId_, const std::string& term_, const std::string& def_);
    Word(int id_, int folderId_, const std::string& term_, const std::string& def_,
         int timesRev_, double avgSc_, std::time_t lastRev_);

    int getId() const;
    int getFolderId() const;
    const std::string& getTerm() const;
    double getAvgScore() const;


    void updateReview(int score);

    // CSV I/O
    std::string toCsv() const;
    static Word   fromCsv(const std::string& line);
    const std::string& getDefinition() const;
    std::time_t getLastReviewedAt() const;
};

#endif // WORD_HPP
