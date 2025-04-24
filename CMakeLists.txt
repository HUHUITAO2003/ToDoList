cmake_minimum_required(VERSION 3.28)
project(ToDoList)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(test)


set(SOURCE_FILES TaskList.cpp TaskList.h Task.cpp Task.h)
add_executable(ToDoList main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(ToDoList core)