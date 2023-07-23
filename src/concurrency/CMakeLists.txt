set(LIB_NAME "concurrency")

add_library(${LIB_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/ConcurrencyDefs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConcurrencyDefs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConcurrencyUtils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ConcurrencyUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Sync.h
)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})