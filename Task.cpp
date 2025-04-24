//
// Created by huita on 18/02/2025.
//

#include "Task.h"

#include <bits/stdc++.h>

void Task::print() const {
    cout << "ID: " << id
            << " | " << (completed ? "[x]" : "[ ]")
            << " - " << title << " - " << endl << description << endl << endl;
}


// Serializza il task in una stringa con formato: id;completed;title;description
string Task::serialize() const {
    string line = to_string(id) + "|" + to_string(static_cast<int>(urgencyLevel)) + "|" + (completed ? "1" : "0") + "|" + title + "|" + description;
    return line;
}

// Deserializza una stringa per creare un oggetto Task
// Effettua la "parsing" manuale dei campi utilizzando string::find e string::substr

//TODO eseguire un controllo durante il caricamento di task da disco se gli id presenti nel file sono già stati occupati, in caso positivo dare all'utente la possibilità
//TODO di scegliere se modificare l'id dei task locali o se modificare l'id dei task che devo andare a caricare
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
