//
// Created by huita on 29/04/2025.
//

#include "ToDoListInterface.h"

void ToDoListInterface::start() {
    int choice;
    do {
        cout << "Inserisci il codice dell'operazione che vuoi effettuare:" << endl;
        cout << "1. Aggiungi una Task" << endl;
        cout << "2. Crea una To Do List" << endl;
        cout << "3. Visualizza le To Do List in locale" << endl;
        cout << "4. Visualizza le To Do List su disco" << endl;
        cout << "5. Carica To Do List da disco" << endl;
        cout << "6. Esci" << endl;
        cin >> choice;
        cin.ignore(); // rimuove \n dal buffer, se no getLine finisce subito

        switch (choice) {
            //Aggiunta Task
            case 1: {
                if(taskListManager.getNumberOfTaskList() == 1) {
                    addTask(0);//aggiunta direttamente nel TaskListLocale
                    cout << "Task aggiunta correttamente." << endl << endl;
                    break;
                }

                cout << "Inserisci il ToDoListID in cui vuoi aggiungere la Task: " << endl;
                string result;
                taskListManager.toString(result);
                cout << result << endl;
                int taskListPosition = taskListManager.getTaskListPosition(getInt());
                while(taskListPosition == -1) {
                    cout << "Errore: ToDoListID non presente" << endl;
                    cout << "Inserisci il ToDoListID in cui vuoi aggiungere la Task: " << endl;
                    taskListPosition = taskListManager.getTaskListPosition(getInt());
                }
                addTask(taskListPosition);
                cout << "Task aggiunta correttamente." << endl << endl;
                break;
            }

            //Aggiungi TaskList
            case 2: {
                addTaskList();
                cout << "To Do List aggiunta correttamente." << endl << endl;
                break;
            }

            //Visualizza TaskList dentro TaskListManager
            case 3: {
                if(taskListManager.getNumberOfTaskList() == 0) {
                    cout << "Non hai nessuna To Do List in locale, prova a crearne una o caricarne una da disco." << endl;
                    break;
                }

                cout << "Elenco dei To Do List in locale:" << endl;
                string taskLists;
                taskListManager.toString(taskLists); //evitare di passare per valore il risultato, essendo se creo stringa dentro toStrng viene poi cancellata.
                cout << taskLists << endl << endl;

                cout << "Inserisci il ToDoListID della To Do List che vuoi visualizzare: " << endl;
                int taskListID;
                cin >> taskListID;
                while (taskListManager.getTaskListPosition(taskListID) == -1) {
                    cout << "Errore: ToDoListID non esistente" << endl;
                    cout << "Inserisci il ToDoListID della To Do List che vuoi inserire: " << endl;
                    taskListID = getInt();
                }
                taskListInterface(taskListManager.getTaskListPosition(taskListID));
                break;
            }

            case 4: {
                string result;
                try {
                    taskListManager.savedTaskListToString(result);
                } catch (const exception &e) {
                    printError(e.what());
                }
                if(result.empty()) {
                    cout << "Non hai nessuna To Do List su disco, prova a salvarne una." << endl;
                    break;
                }
                cout << "Elenco dei To Do List su disco:" << endl;
                cout << result << endl << endl;
                break;
            }

            case 5: {
                string result;
                try {
                    taskListManager.savedTaskListToString(result);
                } catch (const exception &e) {
                    printError(e.what());
                }
                if(result.empty()) {
                    cout << "Non hai nessuna To Do List su disco, prova a salvarne una." << endl;
                    break;
                }

                cout << "Elenco dei To Do List su disco:" << endl; //evitare di passare per valore il risultato, essendo se creo stringa dentro toStrng viene poi cancellata.
                cout << result << endl << endl;

                cout << "Inserisci il ToDoListID della To Do List che vuoi visualizzare: " << endl;
                int taskListID = getInt();
                while (true) {
                    try {
                        taskListManager.loadTaskList(taskListID);
                        break;
                    } catch(const task_list_id_absent &e) {
                        cout << "Errore: ToDoListID non esistente" << endl;
                        cout << "Inserisci il ToDoListID della To Do List che vuoi inserire: " << endl;
                        taskListID = getInt();
                    }
                }
                cout << "To Do List caricata con successo." << endl << endl;
                break;
            }

            case 6: {
                //Esci
                cout << "Uscita dal programma." << endl;
                break;
            }

            default: {
                try {
                    if (cin.fail()) {
                        throw invalid_argument("contenyto invalido");
                    }
                } catch (const invalid_argument &e) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                printError("Opzione non valida. Riprova.");
                break;
            }
        }
    } while (choice != 6);
}

void ToDoListInterface::addTask(int taskListPosition) {
    string title, description;
    int urgencyLevel;
    cout << "Inserisci il titolo del task (non puo' contenere il carattere | ): " << endl;
    getline(cin, title);

    cout << "Inserisci la descrizione (non puo' contenere il carattere | ): " << endl;
    getline(cin, description);

    cout << "Inserisci il livello di urgenza (0: Basso, 1: Medio, 2:Alto, 3.Critico): " << endl;
    urgencyLevel = getInt();

    while(true) {
        try {
            taskListManager.addTask(title, description, urgencyLevel, taskListPosition);
            break;
        } catch (const invalid_title_argument &e) {
            cout << e.what() << endl;
            cout << "Inserisci il titolo del task (non puo' contenere il carattere | ): " << endl;
            getline(cin, title);
        } catch (const invalid_description_argument &e) {
            cout << e.what() << endl;
            cout << "Inserisci la descrizione (non puo' contenere il carattere | ): " << endl;
            getline(cin, description);
        } catch (const invalid_urgencyLevel_argument &e) {
            cout << e.what() << endl;
            cout << "Inserisci il livello di urgenza (0: Basso, 1: Medio, 2:Alto, 3.Critico ): " << endl;
            urgencyLevel = getInt();
        } catch (const invalid_id_argument &e) {
            cout << e.what() << endl;
        }
    }
}

void ToDoListInterface::addTaskList() {
    string name;
    cout << "Inserisci il nome della To Do List che vuoi creare: " << endl;
    getline(cin, name);
    try {
        taskListManager.addTaskList(name);
    } catch (const invalid_name_argument &e) {
        cout << e.what() << endl;
    }
}


void ToDoListInterface::printError(const string &errorMessage) const {
    cerr << errorMessage << endl << endl;
    this_thread::sleep_for(std::chrono::milliseconds(500));
}

int ToDoListInterface::getInt() const {
    int input;
    cin >> input;
    while (cin.fail()) {
        cin.clear(); // Ripristina lo stato di cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora l'input errato
        cout << "Errore: non hai inserito un numero." << endl;
        cin >> input;
    }
    cin.ignore(); // rimuove \n dal buffer, se no getLine finisce subito
    return input;
}


void ToDoListInterface::taskListInterface(int taskListPosition) {
    int choice;
    do {
        cout << "Inserisci il codice dell'operazione che vuoi effettuare:" << endl;
        cout << "1. Aggiungi task" << endl;
        cout << "2. Visualizza task" << endl;
        cout << "3. Completa task" << endl;
        cout << "4. Salva To Do List su disco" << endl;
        cout << "5. Esci" << endl;
        choice = getInt();
        switch (choice) {
            //Aggiungi
            case 1: {
                addTask(taskListPosition);
                cout << "Task aggiunto correttamente." << endl << endl;
                break;
            }

            //Visualizza
            case 2: {
                cout << "Elenco dei task:" << endl;
                string result;
                taskListManager.taskListToString(taskListPosition, result);
                cout << result << endl;
                break;
            }

            //Completa
            case 3: {
                cout << "Elenco dei task:" << endl;
                string result;
                taskListManager.taskListToString(taskListPosition, result);
                cout << result << endl;

                cout << "Inserisci il TaskID che hai completato." << endl;
                int id = getInt();
                try {
                    if (taskListManager.completeTask(taskListPosition, id)) {
                        cout << "Task completato con successo." << endl << endl;
                    } else {
                        printError("Errore: TaskID non presente.");
                    }
                } catch (const out_of_range& e) {
                    printError("Errore: TaskListID non presente");
                    choice = 5;
                    break;
                }
                break;
            }

            case 4: {
                try{
                    taskListManager.saveTaskList(taskListPosition);
                } catch (const exception& e) {
                    printError(e.what());
                    choice = 5;
                    break;
                }
                break;
            }

            case 5: {
                //Esci
                cout << "Uscita dal programma." << endl;
                break;
            }

            default: {
                printError("Opzione non valida. Riprova.");
                break;
            }
        }
    } while (choice != 5);
}




