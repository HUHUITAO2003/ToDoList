//
// Created by huita on 18/02/2025.
//

#include "Task.h"

#include <bits/stdc++.h>

string Task::toString() const {
    return "ID: " + to_string(id) + " | " + (completed ? "[x]" : "[ ]") + " - " + title + " - " + "\n" + description + "\n\n";
}


// Serializza il task in una stringa con formato: id|urgencyLevel|completed|title|description
string Task::serialize() const {
    string line = to_string(id) + "|" + to_string(static_cast<int>(urgencyLevel)) + "|" + (completed ? "1" : "0") + "|" + title + "|" + description;
    return line;
}

// Deserializza una stringa per creare un oggetto Task
// Effettua la "parsing" manuale dei campi
Task Task::deserialize(const string &line) {

    vector<string> v;

    stringstream ss(line);
    while (ss.good()) {
        string substr;
        getline(ss, substr, '|');
        v.push_back(substr);
    }

    // Conversione dei valori
    int id = stoi(v[0]);
    int urgency = stoi(v[1]);
    bool comp = (stoi(v[2]) == 1);
    string titleStr = v[3];
    string descriptionStr = v[4];
    return Task(id, titleStr, descriptionStr, urgency, comp);
}
