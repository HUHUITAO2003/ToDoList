cmake_minimum_required(VERSION 3.28)
project(ToDoList)

set(CMAKE_CXX_STANDARD 17)

add_executable(ToDoList main.cpp
        TaskList.cpp
        TaskList.h
        Task.cpp
        Task.h
)
