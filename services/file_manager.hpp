// include/services/file_manager.hpp

#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include "../include/list.hpp"
#include "../models/folder.hpp"
#include "../models/word.hpp"
#include "../models/quiz.hpp"
#include "../models/question.hpp"

class FileManager {
public:
    /// basePath is where your CSV files live (e.g. "data")
    explicit FileManager(const std::string& basePath = "data");

    // ─── Folder CRUD ───────────────────────────────────────────────────────────
    List<Folder> loadFolders() const;
    void         saveFolders(const List<Folder>& folders) const;
    void         updateFolder(const Folder& folder) const;
    void         deleteFolder(int folderId)        const;

    // ─── Word CRUD (already in use via loadWords/saveWords) ────────────────────
    List<Word>   loadWords()   const;
    void         saveWords(const List<Word>& words) const;

    // ─── Quiz CRUD ─────────────────────────────────────────────────────────────
    List<Quiz>   loadQuizzes() const;
    void         saveQuizzes(const List<Quiz>& quizzes) const;

    // ─── Question CRUD ─────────────────────────────────────────────────────────
    List<Question> loadQuestions() const;
    void           saveQuestions(const List<Question>& questions) const;

private:
    std::string m_base;

    // file paths
    std::string folderFile()   const { return m_base + "/folders.csv"; }
    std::string wordFile()     const { return m_base + "/words.csv"; }
    std::string quizFile()     const { return m_base + "/quizzes.csv"; }
    std::string questionFile() const { return m_base + "/questions.csv"; }

    // simple CSV helpers
    static List<std::string> split(const std::string& line, char delim);
    static std::string       join (const List<std::string>& parts, char delim);
};

#endif // FILE_MANAGER_HPP
