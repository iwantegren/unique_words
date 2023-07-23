cmake_minimum_required(VERSION 3.16.3)

project(benchmark VERSION 1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME} 
benchmark.cpp
../src/utils/LetterTree.h
../src/utils/LetterTree.cpp
../src/utils/Utils.h
../src/utils/Utils.cpp
../src/concurrency/ConcurrencyDefs.h
../src/concurrency/ConcurrencyDefs.cpp
../src/concurrency/ConcurrencyUtils.h
../src/concurrency/ConcurrencyUtils.cpp
../src/concurrency/Sync.h
)

find_package(benchmark REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads benchmark::benchmark)