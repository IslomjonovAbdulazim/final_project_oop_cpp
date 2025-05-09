#ifndef WORD_HPP
#define WORD_HPP

#include <string>
#include <ctime>
#include "../include/list.hpp"

class Word {
private:
    static int            nextId;
    int                    id;
    int                    userId;         // who created it
    std::string            text;
    std::string            meaning;
    std::string            definition;
    std::string            partOfSpeech;
    List<std::string>      examples;
    int                    folderId;
    std::time_t            createdAt;
    std::time_t            lastReviewedAt;
    int                    timesReviewed;
    int                    correctAnswers;
    int                    wrongAnswers;

public:
    // Constructors
    Word();
    Word(int userId,
         const std::string& text,
         const std::string& meaning,
         const std::string& definition,
         const std::string& partOfSpeech,
         int folderId);

    // Getters
    int getId()           const;
    int getUserId()       const;
    std::string getText() const;
    std::string getMeaning() const;
    std::string getDefinition() const;
    std::string getPartOfSpeech() const;
    List<std::string> getExamples() const;
    int getFolderId()     const;
    std::time_t getCreatedAt()   const;
    std::time_t getLastReviewedAt() const;
    int getTimesReviewed() const;
    int getCorrectAnswers() const;
    int getWrongAnswers()   const;

    // Setters
    void setUserId(int uid);
    void setMeaning(const std::string& m);
    void setDefinition(const std::string& d);
    void setPartOfSpeech(const std::string& p);
    void setFolderId(int fid);
    void addExample(const std::string& ex);

    // Review tracking
    void recordReview(bool correct);

    // Print
    void printInfo() const;
    void setText(const std::string& t)      { text   = t; }
};

#endif // WORD_HPP
