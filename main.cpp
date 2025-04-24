#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "TaskList.h"

#include <thread>
#include <chrono>

using namespace std;

void printError(const string &errorMessage) {
    cerr << errorMessage << endl << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main() {
    TaskList todo;
    int choice;
    do {
        cout << "Inserisci il codice dell'operazione che vuoi effettuare:" << endl;
        cout << "1. Aggiungi task" << endl;
        cout << "2. Visualizza task" << endl;
        cout << "3. Completa task" << endl;
        cout << "4. Salva task su disco" << endl;
        cout << "5. Carica task da disco" << endl;
        cout << "6. Esci" << endl;
        cin >> choice;
        cin.ignore(); // rimuove \n dal buffer, se no getLine finisce subito


        switch (choice) {
            case 1: {
                //Aggiungi
                string title, description, urgencyLevelStr;
                int level;
                do {
                    cout << "Inserisci il titolo del task (non puo' contenere il carattere |): ";
                    getline(cin, title);
                } while (title.find("|") != string::npos);
                do {
                    cout << "Inserisci la descrizione (non puo' contenere il carattere |): ";
                    getline(cin, description);
                } while (title.find("|") != string::npos);
                do {
                    cout << "Inserisci il livello di urgenza (0: Basso, 1: Medio, 2:Alto, 3.Critico): ";
                    getline(cin, urgencyLevelStr);
                    try {
                        level = stoi(urgencyLevelStr); // Converte la stringa in intero
                    } catch (const std::invalid_argument &e) {
                        printError("Errore: input non valido!");
                        continue;
                    }
                    if (level < 0 || level > 3) {
                        printError("Errore: valore non compatibile per nessun livello!");
                    }
                } while(level < 0 || level > 3);


                todo.addTask(title, description, level);
                cout << "Task aggiunto correttamente." << endl << endl;
                break;
            }

            case 2: {
                //Visualizza
                cout << "Elenco dei task:" << endl;
                todo.printTasks();
                break;
            }

            case 3: {
                //Complta
                string id;
                todo.printTasks();
                cout << "Inserisci l'ID della task che hai completato." << endl;
                cin >> id;
                try {
                    if (todo.completeTask(stoi(id))) {
                        // Converte la stringa in intero
                        cout << "Task completato con successo." << endl << endl;
                    } else {
                        printError("id task errato.");
                    }
                } catch (const std::invalid_argument &e) {
                    printError("Errore: input non valido!");
                    break;
                }
            }

            case 4: {
                //Salva
                if (todo.save())
                    cout << "Task salvati con successo." << endl << endl;
                else
                    printError("Errore nel salvataggio dei task.");
                break;
            }

            case 5: {
                //Carica
                if (todo.load())
                    cout << "Task caricati con successo." << endl << endl;
                else
                    printError("Errore nel caricamento dei task.");
                break;
            }

            case 6: {
                //Esci
                cout << "Uscita dal programma." << endl;
                break;
            }

            default: {
                try {
                    if (std::cin.fail()) {
                        throw std::invalid_argument("contenyto invalido");
                    }
                } catch (const std::invalid_argument &e) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                printError("Opzione non valida. Riprova.");
                break;
            }
        }
    } while (choice != 6);
    return 0;
}
