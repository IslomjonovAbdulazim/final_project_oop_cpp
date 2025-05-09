#ifndef FOLDER_HPP
#define FOLDER_HPP

#include <string>
#include <ctime>
#include "../include/list.hpp"

class Folder {
private:
    static int        nextId;
    int               id;
    int               userId;        // who created it
    std::string       name;
    std::string       description;
    int               wordsCount;
    std::time_t       createdAt;
    std::time_t       lastReviewedAt;
    std::time_t       lastUpdatedAt;
    List<int>         quizzes;
    double            totalQuizScore;
    double            averageQuizScore;

public:
    // Constructors
    Folder();
    Folder(int userId, const std::string& name, const std::string& description);

    // Getters
    int getId()               const;
    int getUserId()           const;
    std::string getName()     const;
    std::string getDescription() const;
    int getWordsCount()       const;
    std::time_t getCreatedAt()     const;
    std::time_t getLastReviewedAt() const;
    std::time_t getLastUpdatedAt()  const;
    List<int> getQuizzes()    const;
    double getAverageQuizScore() const;

    // Setters
    void setUserId(int uid);
    void setName(const std::string& name);
    void setDescription(const std::string& desc);
    void setWordsCount(int count);
    void setLastReviewedAt(std::time_t t);

    // Quiz‐score methods
    void addQuizScore(int score);
    bool removeQuizScore(int score);
    void clearQuizScores();

    // Print
    void printInfo() const;
};

#endif // FOLDER_HPP
