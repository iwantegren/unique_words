cmake_minimum_required(VERSION 3.16.3)

project(unique_words VERSION 1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
main.cpp
LetterTree.h
LetterTree.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)