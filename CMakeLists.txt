cmake_minimum_required(VERSION 3.16.3)

project(unique_words VERSION 1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
    src/main.cpp
)

add_subdirectory(src/concurrency)
target_link_libraries(${PROJECT_NAME} PRIVATE concurrency)

add_subdirectory(src/utils)
target_link_libraries(${PROJECT_NAME} PRIVATE utils)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)