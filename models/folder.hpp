#ifndef FOLDER_HPP
#define FOLDER_HPP

#include <string>
#include <ctime>
#include <utility>
#include "../include/list.hpp"

class Folder {
private:
    int                              id;
    int                              userId;
    std::string                      name;
    std::string                      description;
    int                              wordsCount;
    List<std::pair<int,int>>         quizResults;    // (correct, total)
    std::time_t                      createdAt;
    std::time_t                      lastReviewedAt;
    std::time_t                      lastUpdatedAt;

    static int                       nextId;

public:
    // New-folder constructor
    Folder(int userId, const std::string& name, const std::string& desc);

    // CSV-loading constructor (results moved in)
    Folder(int id, int userId,
           const std::string& name, const std::string& desc,
           int wordsCount,
           List<std::pair<int,int>> results,
           std::time_t created, std::time_t lastRev, std::time_t lastUpd);

    // Getters
    int    getId()            const { return id; }
    int    getUserId()        const { return userId; }
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    int    getWordsCount()    const { return wordsCount; }
    std::size_t getQuizCount() const { return quizResults.size(); }
    std::time_t getLastReviewedAt() const { return lastReviewedAt; }
    std::time_t getLastUpdatedAt()  const { return lastUpdatedAt; }

    // Setters
    void setName(const std::string& n)       { name = n;       lastUpdatedAt = std::time(nullptr); }
    void setDescription(const std::string& d){ description = d; lastUpdatedAt = std::time(nullptr); }
    void setWordsCount(int wc)               { wordsCount = wc; lastUpdatedAt = std::time(nullptr); }

    // Add one quiz result
    void addQuizResult(int correct, int total);

    // True average percentage: mean(correct/total)*100
    double averageScore() const;

    // CSV I/O
    std::string toCsv() const;
    static Folder fromCsv(const std::string& line);
};

#endif // FOLDER_HPP
