#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include "../include/list.hpp"
#include "../models/user.hpp"
#include "../models/folder.hpp"
#include "../models/word.hpp"
#include "../models/quiz.hpp"
#include "../models/question.hpp"

class FileManager {
private:
    std::string baseDir;

    template<typename T>
    List<T> loadAll(const std::string& filename, T(*fromCsv)(const std::string&));

    template<typename T>
    void saveAll(const std::string& filename, const List<T>& items);

    void ensureCsv(const std::string& filename, const std::string& header);

public:
    FileManager(const std::string& dataDir);

    List<User>     loadUsers();
    void           saveUsers(const List<User>&);

    List<Folder>   loadFolders();
    void           saveFolders(const List<Folder>&);

    List<Word>     loadWords();
    void           saveWords(const List<Word>&);

    List<Quiz>     loadQuizzes();
    void           saveQuizzes(const List<Quiz>&);

    List<Question> loadQuestions();
    void           saveQuestions(const List<Question>&);
};

#endif // FILE_MANAGER_HPP
