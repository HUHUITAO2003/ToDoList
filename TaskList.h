//
// Created by huita on 18/02/2025.
//

#ifndef TASKLIST_H
#define TASKLIST_H

#include "Task.h"

using namespace std;

class invalid_name_argument : public std::invalid_argument {public: explicit invalid_name_argument(const std::string& msg) : std::invalid_argument(msg) {}};
class task_id_absent : public runtime_error {public: explicit task_id_absent(const string& msg) : runtime_error(msg) {}};

class TaskList {
private:
    vector<Task> tasks;
    int taskListID;
    static inline int nextID = 1;
    string name;

    bool isIdOccupied(int idToCheck) const;

public:
    TaskList(int taskListID, string &name) : taskListID(taskListID), name(name) {
        if(name.find("|") != string::npos) throw invalid_name_argument("Errore: il nome che hai inserito comtiene il simbolo \"|\"");
    }

    static int getNextId() { return nextID; }
    void static setNextId(int newNextID) { nextID = newNextID; }
    const string &getName() const { return name; }
    int getTaskListID() const { return taskListID; }
    int getNumberOfTask() const { return tasks.size(); }
    Task getTask(int position) { return tasks.at(position); }
    Task getTaskByID(int position) const;
    int getNumberOfNotCompletedTask() const ;

    // Aggiunge un task tramite titolo e descrizione
    void addTask(const string &title, const string &description, int urgencyLevel);

    // Aggiunge un task già costruito e aggiorna nextId se necessario
    void addTask(Task &task);

    bool deleteTask(int taskListID);

    vector<int> taskContains(const string &word) const;

    bool modifyTask(int taskListID, const string &newTitle, const string &newDesciption, int urgencyLevel, bool completed);

    vector<int> getXXXurgencyLevelTaskPosition(int urgencyLevel) const;

    bool completeTask(int ID);

    string toString() const;

    vector<string> serialize() const;

    void deserialize(const vector<string> &lines);
};


#endif //TASKLIST_H
