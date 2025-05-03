cmake_minimum_required(VERSION 3.28)

add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR, ./lib/googletest/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_TEST_FILES runAllTests.cpp TaskTests.cpp TaskListTests.cpp
        TaskListManagerTests.cpp)
add_executable(runTests ${SOURCE_TEST_FILES})
target_link_libraries(runTests gtest gtest_main core)