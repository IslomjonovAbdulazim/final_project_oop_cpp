cmake_minimum_required(VERSION 3.29)
project(final_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(final_project main.cpp
        services/file_manager.hpp
        services/file_manager.cpp
        models/folder.hpp
        models/folder.cpp
        models/word.hpp
        models/word.cpp
        models/quiz.hpp
        models/quiz.cpp
        models/question.hpp
        models/question.cpp
        include/list.hpp
        services/console_service.hpp
        services/console_service.cpp
        services/menu_service.cpp
        services/menu_service.hpp
        models/user.cpp
        models/user.hpp
        services/auth_service.cpp
        services/auth_service.hpp
)
