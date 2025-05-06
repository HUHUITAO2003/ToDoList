//
// Created by huita on 29/04/2025.
//

#ifndef TASKLISTMANAGER_H
#define TASKLISTMANAGER_H

#include "TaskList.h"
#include <fstream>
#include <iostream>

class task_list_id_occupied : public runtime_error {public: explicit task_list_id_occupied(const string& msg) : runtime_error(msg) {}};
class cannot_open_file : public runtime_error {public: explicit cannot_open_file(const string& msg) : runtime_error(msg) {}};
class file_corrupted : public exception { private: string msg; public: file_corrupted(const std::string& msg) : msg(msg) {} const char* what() const noexcept override { return msg.c_str(); }};
class task_list_id_absent : public runtime_error {public: explicit task_list_id_absent(const string& msg) : runtime_error(msg) {}};

class TaskListManager {
private:
    int nextTaskListID = 1;
    vector<TaskList> taskLists;
    static inline string path = "./SavedToDo.txt";

    void readFileHeader(const string&header, vector<int> &taskListsIDs, vector<int> &startPositions, vector<int> &sizes) const;

public:
    TaskListManager();
    ~TaskListManager();

    static void setPath(const string &newPath) { path = newPath; }

    void addTaskList(string &name);
    void addTaskList(TaskList &tasksList);
    void addTask(const string &title, const string &description, int urgencyLevel, int taskListPosition = 0) {
        taskLists[taskListPosition].addTask(title, description, urgencyLevel);
    }
    void addTask(Task &task, int taskListPosition = 0) {
        taskLists[taskListPosition].addTask(task);
    }
    TaskList getTaskList(int taskListID) const;
    string toString() const;
    string savedTaskListToString() const;
    int getTaskListPosition(int taskListID) const;
    int getNumberOfTaskList() const { return taskLists.size(); }
    int getNextTaskListID() const { return nextTaskListID; }
    bool deleteTask(int taskID, int taskListPosition);
    bool modifyTask(int taskID, int taskListPosition, const string &newTitle, const string &newDesciption, int urgencyLevel, bool completed);
    vector<int> findWord(const string &word) const;
    int getNumberOfNotCompletedTask() const;

    string taskListToString(int taskListPosition);
    bool completeTask(int taskListPosition, int taskId);

    void loadTaskList(int taskListID);
    void saveTaskList(int taskListPosition) const;
};



#endif //TASKLISTMANAGER_H
