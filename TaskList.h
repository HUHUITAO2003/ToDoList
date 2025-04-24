//
// Created by huita on 18/02/2025.
//

#ifndef TASKLIST_H
#define TASKLIST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include "Task.h"

using namespace std;

class TaskList {
private:
    vector<Task> tasks;
    int nextId;
    string path = "./SavedToDo.txt";

    bool isIdOccupied(int idToCheck) const;

public:
    explicit TaskList(int nextId = 1) : nextId(nextId) {}

    // Aggiunge un task tramite titolo e descrizione
    void addTask(const string &title, const string &description, int urgencyLevel);

    // Aggiunge un task già costruito e aggiorna nextId se necessario
    void addTask(Task &task);

    bool completeTask(int ID);

    void printTasks() const;

    bool save() const;

    bool load();
};


#endif //TASKLIST_H
