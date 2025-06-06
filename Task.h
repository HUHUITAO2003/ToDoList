//
// Created by huita on 18/02/2025.
//

#ifndef TASK_H
#define TASK_H

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

using namespace std;

class invalid_id_argument : public invalid_argument {public: explicit invalid_id_argument(const string& msg) : invalid_argument(msg) {}};
class invalid_urgencyLevel_argument : public invalid_argument {public: explicit invalid_urgencyLevel_argument(const string& msg) : invalid_argument(msg) {}};
class invalid_title_argument : public invalid_argument {public: explicit invalid_title_argument(const string& msg) : invalid_argument(msg) {}};
class invalid_description_argument : public invalid_argument {public: explicit invalid_description_argument(const string& msg) : invalid_argument(msg) {}};
class parse_error : public runtime_error {public: explicit parse_error(const string& msg) : runtime_error(msg) {}};

class Task {
private:
    int id;
    bool completed;
    string title;
    string description;
    int urgencyLevel;

public:
    static inline const vector<string> urgencyLevels = {"Basso", "Medio", "Alto", "Critico"};

    Task(int id, const string &title, const string &description, int urgencyLevel, bool completed = false)
        : id(id), title(title), description(description), urgencyLevel(urgencyLevel), completed(completed) {
        if( id < 0 ) throw invalid_id_argument("Errore: valore ID non negativo");
        if( title.find("|") != string::npos ) throw invalid_title_argument("Errore: titolo contenente il simbolo |");
        if( description.find("|") != string::npos ) throw invalid_description_argument("Errore: titolo contenente il simbolo |");
        if( urgencyLevel < 0 || urgencyLevel > 3 ) throw invalid_urgencyLevel_argument("Errore: livello di urgenza non valido");
    }

    int getId() const { return id; }
    bool isCompleted() const { return completed; }
    const string &getTitle() const { return title; }
    int getUrgencyLevel() const { return urgencyLevel; }
    const string &getDescription() const { return description; }
    bool contains(const string &word) const;

    void setId(int id) { this->id = id; }
    void setCompleted(bool value) { completed = value; }
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
