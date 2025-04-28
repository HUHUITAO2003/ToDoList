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

int getInt() {
    int input;
    cin >> input;
    while(cin.fail()) {
        cout << "Errore: non hai inserito un numero." << endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cin >> input;
    }
    cin.ignore(); // rimuove \n dal buffer, se no getLine finisce subito
    return input;
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
                        todo.addTask(title, description, urgencyLevel);
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
                    }
                }
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
