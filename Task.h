//
// Created by huita on 18/02/2025.
//

#ifndef TASK_H
#define TASK_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Task {
private:
    int id;
    bool completed;
    string title;
    string description;
    int urgencyLevel;

public:
    Task(int id, const string &title, const string &description, int urgencyLevel, bool completed = false)
        : id(id), title(title), description(description), urgencyLevel(urgencyLevel), completed(completed) {
    }

    int getId() const { return id; }
    bool isCompleted() const { return completed; }
    const string &getTitle() const { return title; }
    const int &getUrgencyLevel() const { return urgencyLevel; }
    const string &getDescription() const { return description; }

    void setId(int id) { this->id = id; }
    void setCompleted(bool value) { this->completed = value; }
    void setUrgencyLevel(int urgencyLevel) { this->urgencyLevel = urgencyLevel; }

    // Stampa i dettagli del task
    string toString() const;

    // Serializza il task in una stringa con formato: id;completed;title;description
    string serialize() const;

    // Deserializza una stringa per creare un oggetto Task
    // Effettua la "parsing" manuale dei campi utilizzando string::find e string::substr
    static Task deserialize(const string &line);
};


#endif //TASK_H
