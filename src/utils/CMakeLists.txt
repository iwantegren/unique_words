set(LIB_NAME "utils")

add_library(${LIB_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/LetterTree.h
    ${CMAKE_CURRENT_SOURCE_DIR}/LetterTree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})