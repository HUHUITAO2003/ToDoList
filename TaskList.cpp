//
// Created by huita on 18/02/2025.
//

#include "TaskList.h"

#include <thread>
#include <chrono>
#include <bits/fs_fwd.h>

// Aggiunge un task tramite titolo e descrizione
void TaskList::addTask(const string &title, const string &description, int urgencyLevel) {
    tasks.push_back(Task(nextId, title, description, urgencyLevel));
    nextId++;
}

// Aggiunge un task già costruito e aggiorna nextId se necessario
void TaskList::addTask(Task &task) {
    if (isIdOccupied(task.getId())) {
        task.setId(nextId);
    } else {
        if(nextId < task.getId()) {
            nextId = task.getId();
        }
    }
    nextId++;
    tasks.push_back(task);
}

// Stampa tutti i task a video
void TaskList::printTasks() const {
    if (tasks.empty()) {
        cout << "La lista non contiene alcun task." << endl;
    } else {
        bool printed = false;
        for (const auto &task : tasks) {
            if(task.getUrgencyLevel() == 3) {
                if(!printed) {
                    cout<<"Task di urgenza Critica: " <<endl;
                    printed = true;
                }
                cout << task.toString();
            }
        }
        printed = false;
        for (const auto &task : tasks) {
            if(task.getUrgencyLevel() == 2) {
                if(!printed) {
                    cout<<"Task di urgenza Alta: " <<endl;
                    printed = true;
                }
                cout << task.toString();
            }
        }
        printed = false;
        for (const auto &task : tasks) {
            if(task.getUrgencyLevel() == 1) {
                if(!printed) {
                    cout<<"Task di urgenza Media: " <<endl;
                    printed = true;
                }
                cout << task.toString();
            }
        }
        printed = false;
        for (const auto &task : tasks) {
            if(task.getUrgencyLevel() == 0) {
                if(!printed) {
                    cout<<"Task di urgenza Bassa: " <<endl;
                    printed = true;
                }
                cout << task.toString();
            }
        }
    }
}

// Salva su disco l'elenco dei task nel file specificato
bool TaskList::save() const {
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "Errore nell'aprire il file per la scrittura." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }
    for (const auto &task : tasks) {
        file << task.serialize() << "\n";
    }
    file.close();
    return true;
}



// Carica da disco l'elenco dei task
bool TaskList::load() {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Errore nell'aprire il file per la lettura." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return false;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty())
            continue;
        try {
            Task t = Task::deserialize(line);
            addTask(t);
        } catch (exception &e) {
            cerr << "Errore nel parsing della riga: \"" << line << "\"" << endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    file.close();
    return true;
}

bool TaskList::isIdOccupied(int idToCheck) const{
    for (const auto& task : tasks) {
        if (task.getId() == idToCheck) {
            return true;
        }
    }
    return false;
}

bool TaskList::completeTask(int id) {
    for (auto &task : tasks) {
        if(task.getId() == id) {
            task.setCompleted(true);
            return true;
        }
    }
    return false;
}