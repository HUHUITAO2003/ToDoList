//
// Created by huita on 29/04/2025.
//

#ifndef TODOLISTINTERFACE_H
#define TODOLISTINTERFACE_H

#include "TaskListManager.h"
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class ToDoListInterface {
private:
    TaskListManager taskListManager;

public:
    void start();
    void printError(const string &errorMessage) const;
    int getInt() const;
    void addTask(int taskListPosition = 0);
    void addTaskList();
    void taskListInterface(int taskListIDposition);
};



#endif //TODOLISTINTERFACE_H
