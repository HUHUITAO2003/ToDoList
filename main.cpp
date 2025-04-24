#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "TaskList.h"


using namespace std;

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

                cout << "Inserisci il titolo del task: ";
                getline(cin, title);
                cout << "Inserisci la descrizione: ";
                getline(cin, description);
                cout << "Inserisci il livello di urgenza (0: Basso, 1: Medio, 2:Alto, 3.Critico): ";
                getline(cin, urgencyLevelStr);
                try {
                    level = stoi(urgencyLevelStr); // Converte la stringa in intero
                } catch (const std::invalid_argument &e) {
                    cerr << "Errore: input non valido!" << endl;
                    break;
                }
                if (level < 0 || level > 3) {
                    cout << "Errore: valore non compatibile pe nessun livello!" << endl << endl;
                    break;
                }

                todo.addTask(title, description, level);
                cout << "Task aggiunto correttamente." << endl;
                break;
            }

            case 2: {
                //Visualizza
                cout << "Elenco dei task:" << endl;
                todo.printTasks();
                break;
            }

            case 3: {
                string id;
                todo.printTasks();
                cout << "Inserisci l'ID della task che hai completato." << endl;
                cin >> id;
                try {
                    if (todo.completeTask(stoi(id))) {
                        // Converte la stringa in intero
                        cout << "Task completato con successo." << endl;
                    } else {
                        cout << "id task errato." << endl;
                    }
                } catch (const std::invalid_argument &e) {
                    cerr << "Errore: input non valido!" << endl;
                    break;
                }
            }

            case 4: {
                //Salva
                if (todo.save())
                    cout << "Task salvati con successo." << endl;
                else
                    cout << "Errore nel salvataggio dei task." << endl;
                break;
            }

            case 5: {
                //Carica
                if (todo.load())
                    cout << "Task caricati con successo." << endl;
                else
                    cout << "Errore nel caricamento dei task." << endl;
                break;
            }

            case 6: {
                //Esci
                cout << "Uscita dal programma." << endl;
                break;
            }

            default: {
                //input non valido
                cout << "Opzione non valida. Riprova." << endl;
            }
        }
    } while (choice != 6);
    return 0;
}
