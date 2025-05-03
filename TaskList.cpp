//
// Created by huita on 18/02/2025.
//

#include "TaskList.h"

Task TaskList::getTaskByID(int targetID) const {
    for(auto const &t : tasks) {
        if(t.getId() == targetID) {
            return t;
        }
    }
    throw task_id_absent("Errore: TaskID non presente");
}

int TaskList::getNumberOfNotCompletedTask() const {
    int count = 0;
    for(auto const &t : tasks) {
        if(!t.isCompleted()) {
            count++;
        }
    }
    return count;
}

// Aggiunge un task tramite titolo e descrizione
void TaskList::addTask(const string &title, const string &description, int urgencyLevel) {
    tasks.emplace_back(nextID, title, description, urgencyLevel);
    nextID++;
}

// Aggiunge un task già costruito e aggiorna nextID se necessario
void TaskList::addTask(Task &task) {
    if (isIdOccupied(task.getId())) {
        task.setId(nextID);
    } else {
        if (nextID < task.getId()) {
            nextID = task.getId();
        }
    }
    nextID++;
    tasks.push_back(task);
}

// Stampa tutti i task a video
void TaskList::toString(string &result) const {
    result = "";
    if (tasks.empty()) {
        return;
    }
    for (int i = Task::urgencyLevels.size() - 1; i >= 0; i--) {
        vector<int> positions;
        getXXXurgencyLevelTaskPosition(i, positions);
        if (!positions.empty()) {
            result += "Task di urgenza " + Task::urgencyLevels[i] + ": \n";
            for (int j = 0; j < positions.size(); j++) {
                result += tasks[positions[j]].toString() + "\n\n";
            }
        }
    }
}


void TaskList::getXXXurgencyLevelTaskPosition(int urgencyLevel, vector<int> &positions) const {
    positions.clear();
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].getUrgencyLevel() == urgencyLevel) {
            positions.emplace_back(i);
        }
    }
}


// Salva su disco l'elenco dei task nel file specificato
void TaskList::serialize(vector<string> &lines) const {
    lines.clear();
    lines.emplace_back(to_string(taskListID) + "|" + name);
    for(int i = 0 ; i < tasks.size() ; i++) {
        lines.push_back(tasks[i].serialize());
    }
}


// Carica da disco l'elenco dei task
void TaskList::deserialize(const vector<string> &lines) {
    stringstream ss(lines[0]);
    try {
        string newTaskListId, newName;
        getline(ss, newTaskListId, '|');
        getline(ss, newName, '|');
        taskListID = stoi(newTaskListId);
        name = newName;
    } catch (exception &e) {
        throw parse_error("Errore: parsing dell'intestazione della ToDoList fallito");
    }
    for(int i = 1 ; i < lines.size() ; i++) {
        try {
            Task t = Task::deserialize(lines[i]);
            addTask(t);
        } catch (exception &e) {
            throw parse_error("Errore: parsing della riga: \"" + lines[i] + "\" fallito");
        }
    }
}

bool TaskList::deleteTask(int taskListID) {
    for (int i = 0 ; i < tasks.size() ; i++) {
        if (tasks[i].getId() == taskListID) {
            tasks.erase(tasks.begin() + i);
            return true;
        }
    }
    return false;
}

void TaskList::taskContains(const string &word, vector<int> &taskIDs) const {
    taskIDs.clear();
    for(int i = 0 ;  i < tasks.size() ; i++) {
        if(tasks[i].contains(word)) {
            taskIDs.emplace_back(tasks[i].getId());
        }
    }
}

bool TaskList::modifyTask(int taskID, const string &newTitle, const string &newDesciption, int urgencyLevel,
                          bool completed) {
    if(deleteTask(taskID) == false) {
        return false;
    }
    tasks.emplace_back(taskID, newTitle, newDesciption, urgencyLevel, completed);
    return true;
}

bool TaskList::isIdOccupied(int idToCheck) const {
    for (const auto &task: tasks) {
        if (task.getId() == idToCheck) {
            return true;
        }
    }
    return false;
}

bool TaskList::completeTask(int id) {
    for (auto &task: tasks) {
        if (task.getId() == id) {
            task.setCompleted(true);
            return true;
        }
    }
    return false;
}

