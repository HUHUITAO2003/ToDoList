//
// Created by huita on 29/04/2025.
//

#include "TaskListManager.h"

#include <io.h>

void TaskListManager::addTaskList(string &name) {
    taskLists.emplace_back(nextTaskListID, name);
    nextTaskListID++;
}

void TaskListManager::addTaskList(TaskList &taskList) {
    for (auto const &t : taskLists) {
        if(t.getTaskListID() == taskList.getTaskListID()) {
            throw task_list_id_occupied("Errore: TaskListID gia' occupato");
        }
    }
    taskLists.push_back(taskList);
    if( taskList.getTaskListID() <= nextTaskListID) nextTaskListID = taskList.getTaskListID() + 1;
}

void TaskListManager::toString(string &result) const {
    result = "";
    for(auto const &list : taskLists) {
        result += "ToDoListID:" + to_string(list.getTaskListID()) + " - " + list.getName() + "\n";
    }
}

int TaskListManager::getTaskListPosition(int taskListID) const {
    for (int i = 0 ; i < taskLists.size() ; i++) {
        if(taskLists[i].getTaskListID() == taskListID) {
            return i;
        }
    }
    return -1;
}

void TaskListManager::taskListToString(int taskListPosition, string &result) {
    taskLists.at(taskListPosition).toString(result);
}

bool TaskListManager::completeTask(int taskListPosition, int taskId) {
    return taskLists.at(taskListPosition).completeTask(taskId);
}

void TaskListManager::loadTaskList(int taskListID) {
    ifstream file(path);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }

    //ottenere tutti gli id dei task list salvai e la linea in cui iniziano.
    string nextIDsStr, header;
    getline(file, nextIDsStr);
    getline(file, header);
    vector<int> taskListsIDs, startPositions, sizes;
    if(!header.empty()) {
        readFileHeader(header, taskListsIDs, startPositions, sizes);
    }

    int index = -1;
    for(int i = 0 ; i < taskListsIDs.size() ; i++) {
        if(taskListsIDs[i] == taskListID) {
            index = i;
        }
    }
    if(index == -1) throw task_list_id_absent("Errore: ToDoListID mancante su disco.");

    int contatore = 3;
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        if (contatore == startPositions[index]) {
            while(contatore <= startPositions[index] + sizes[index]) {
                lines.push_back(line);
                getline(file, line);
                contatore++;
            }
        }
        contatore++;
    }
    int taskListPosition = getTaskListPosition(taskListID);
    if(taskListPosition == -1) {
        string name;
        TaskList t(0, name);
        t.deserialize(lines);
        addTaskList(t);
    } else {
        taskLists[taskListPosition].deserialize(lines);
    }
}

void TaskListManager::saveTaskList(int taskListPosition) const {
    ifstream file(path);
    if (!file) {
        file.close();
        throw cannot_open_file("Errore: apertura file fallito.");
    }

    TaskList taskList = taskLists[taskListPosition];

    //ottenere tutti gli id dei task list salvati e la linea in cui iniziano.
    string nextIDsStr, header;
    getline(file, nextIDsStr);
    getline(file, header);
    vector<int> taskListsIDs, startPositions, sizes;
    if(!header.empty()) {
        readFileHeader(header, taskListsIDs, startPositions, sizes);
    }
    int index = -1; //ottiene la posizione se taskList già salvato su disco
    for(int i = 0 ; i < taskListsIDs.size() ; i++) {
        if(taskListsIDs[i] == taskList.getTaskListID()) {
            index = i;
        }
    }

    string tempFilePath = "./temp.txt";
    ofstream tempFile(tempFilePath);
    vector<string> lines;
    taskList.serialize(lines);
    //prima volta che viene salvato
    if(index == -1) {
        int startPosition = 0;
        if(sizes.empty()) {
            startPosition = 3;
        } else {
            startPosition = startPositions.back() + sizes.back() + 1;
        }
        header += to_string(taskList.getTaskListID()) + ":" + to_string(startPosition) + ":" + to_string(taskList.getNumberOfTask()) + "|";
        tempFile << nextIDsStr << endl << header << endl;
        string line;
        int contatore = 3;
        while (getline(file, line)) {
            tempFile << line << "\n";
            contatore++;
        }
        for (const auto& line : lines) {
            tempFile << line << "\n";
        }
    } else {
        //aggiustamenti degli start position esizes
        for(int i = (taskListsIDs.size() - 1) ; i > index ; i--) {
            startPositions[i] += (taskList.getNumberOfTask() - sizes[index]);
        }
        int originaleSize = sizes[index];
        sizes[index] = taskList.getNumberOfTask();
        header = "";
        for(int i = 0 ; i < taskListsIDs.size() ; i++) {
            header += to_string(taskListsIDs[i]) + ":" + to_string(startPositions[i]) + ":" + to_string(sizes[i]) + "|";
        }
        tempFile << nextIDsStr << endl << header << endl;


        int contatore = 3;
        string line;
        while (getline(file, line)) {
            if (contatore == startPositions[index]) {
                for (const auto& newb : lines) {
                    tempFile << newb << "\n";
                }
                for(int j = 0 ; j < originaleSize ; j++) {
                    getline(file, line);
                }
                contatore += sizes[index] + 1;
            } else {
                tempFile << line << "\n";
                contatore++;
            }
        }
    }
    file.close();
    tempFile.close();
    std::remove(path.c_str());
    std::rename(tempFilePath.c_str(), path.c_str());
}

void TaskListManager::savedTaskListToString(string &result) const {
    ifstream file(path);
    if (!file) {
        throw cannot_open_file("Errore: apertura file fallita.");
    }

    string nextIDsStr, header;
    getline(file, nextIDsStr);
    getline(file, header);
    vector<int> taskListsIDs, startPositions, sizes;
    if (!header.empty()) {
        readFileHeader(header, taskListsIDs, startPositions, sizes);
    }

    result = "";
    string line;
    int index = 0, contatore = 3;
    while (getline(file, line)) {
        // Stampa ogni riga del file
        if(contatore == startPositions[index]) {
            stringstream ss(line);
            string taskListID, name;
            while (getline(ss, taskListID, '|')) {
                getline(ss, name, '|');
                result += "ToDoListID:" + taskListID + " - " + name + "\n";
            }
            index++;
        }
        contatore++;
    }
    file.close();
}

void TaskListManager::readFileHeader(const string &header, vector<int> &taskListsIDs, vector<int> &startPositions, vector<int> &sizes) const {
    stringstream ss(header);
    string idStr, sizeStr, positionStr;
    while (getline(ss, idStr, ':')) {
        getline(ss, positionStr, ':');
        getline(ss, sizeStr, '|');
        try {
            int id = stoi(idStr);
            int size = stoi(sizeStr);
            int position = stoi(positionStr);
            taskListsIDs.push_back(id);
            sizes.push_back(size);
            startPositions.push_back(position);
        } catch (const exception& e) {
            string a = e.what();
            throw file_corrupted("Errore: file su disco corrotto.");
        }
    }
}

TaskListManager::TaskListManager() {
    string name = "Locale";
    taskLists.emplace_back(0, name);
    ifstream file(path);
    if (!file) {
        throw cannot_open_file("Errore: apertura file fallita.");
    }

    try {
        string nextIDsStr;
        if(getline(file, nextIDsStr)) {
            stringstream ss(nextIDsStr);
            string savedNextTaskListID, savedNextTaskID;
            getline(ss, savedNextTaskListID, '|');
            getline(ss, savedNextTaskID, '|');
            nextTaskListID = stoi(savedNextTaskListID);
            taskLists[0].setNextId(stoi(savedNextTaskID));
        } else {
            throw file_corrupted("Errore: file corrotto, nextIDs non presenti.");
        }
    } catch (const std::invalid_argument&) {
        throw file_corrupted("Errore: file corrotto, argomento non valido.");
    } catch (const std::out_of_range&) {
        throw file_corrupted("Errore: file corrotto, argomento non valido");
    }
    file.close();
}

TaskListManager::~TaskListManager() {
    //Aggiustamento nextIDs salvati sul disco
    ifstream file(path);
    string nextIDsStr;
    getline(file, nextIDsStr);

    string tempFilePath = "./temp.txt";
    ofstream tempFile(tempFilePath);
    string line;
    tempFile << to_string(nextTaskListID) + "|" + to_string(TaskList::getNextId()) + "|\n";
    while (getline(file, line)) {
        tempFile << line << "\n";
    }
    file.close();
    tempFile.close();
    std::remove(path.c_str());
    std::rename(tempFilePath.c_str(), path.c_str());
}

TaskList TaskListManager::getTaskList(int taskListID) const {
    for(const auto &t: taskLists) {
        if(t.getTaskListID() == taskListID) {
            return t;
        }
    }
    throw task_list_id_absent("Errore: ToDoListID non presente");
}