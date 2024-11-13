#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int TABLE_SIZE = 101;  
const int EMPTY = -1;        
const int DELETED = -2;     

struct HashSet {
    int table[TABLE_SIZE];  // Хеш-таблица для хранения элементов
    int size;               // Количество элементов в множестве

    HashSet() : size(0) {
        // Инициализация пустыми значениями
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = EMPTY;
        }
    }

    // Хеш-функция
    int hash(int value) const {
        return (value % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }

    // Добавление элемента
    void add(int value) {
        if (contains(value)) return;  
        int index = hash(value);
        int startIndex = index;
        while (table[index] != EMPTY && table[index] != DELETED) {
            index = (index + 1) % TABLE_SIZE;  
            if (index == startIndex) return;   
        }
        table[index] = value;
        size++;
    }

    // Удаление элемента
    void remove(int value) {
        int index = hash(value);
        int startIndex = index;
        while (table[index] != EMPTY) {
            if (table[index] == value) {
                table[index] = DELETED;
                size--;
                return;
            }
            index = (index + 1) % TABLE_SIZE;
            if (index == startIndex) return;
        }
    }

    // Проверка наличия элемента
    bool contains(int value) const {
        int index = hash(value);
        int startIndex = index;
        while (table[index] != EMPTY) {
            if (table[index] == value) return true;
            index = (index + 1) % TABLE_SIZE;
            if (index == startIndex) break;
        }
        return false;
    }

    // Вывод множества
    void print() const {
        cout << "Set: ";
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i] != EMPTY && table[i] != DELETED) {
                cout << table[i] << " ";
            }
        }
        cout << endl;
    }

    // Запись множества в файл
    void saveToFile() const {
        ofstream outFile("set_data.txt");
        if (outFile.is_open()) {
            for (int i = 0; i < TABLE_SIZE; i++) {
                if (table[i] != EMPTY && table[i] != DELETED) {
                    outFile << table[i] << " ";
                }
            }
            outFile.close();
            cout << "Set saved to file." << endl;
        } else {
            cout << "Error opening file for writing!" << endl;
        }
    }

    // Загрузка множества из файла
    void loadFromFile() {
        ifstream inFile("set_data.txt");
        if (inFile.is_open()) {
            size = 0;
            for (int i = 0; i < TABLE_SIZE; i++) {
                table[i] = EMPTY;
            }
            int value;
            while (inFile >> value) {
                add(value);
            }
            inFile.close();
            cout << "Set loaded from file." << endl;
        } else {
            cout << "Error opening file for reading!" << endl;
        }
    }
};

// Обработка команд
void processCommand(HashSet &s, const string &commandLine) {
    if (commandLine.find("SETADD") == 0) {
        int value = stoi(commandLine.substr(7));
        s.add(value);
    } else if (commandLine.find("SETDEL") == 0) {
        int value = stoi(commandLine.substr(7));
        s.remove(value);
    } else if (commandLine.find("SET_AT") == 0) {
        if (commandLine.length() > 7) { 
            int value = stoi(commandLine.substr(7));
            cout << (s.contains(value) ? "Yes" : "No") << endl;
        }
    } else if (commandLine == "PRINT") {
        s.print();
    } else if (commandLine == "SAVE") {
        s.saveToFile();
    } else if (commandLine == "LOAD") {
        s.loadFromFile();
    } else {
        cout << "Unknown command." << endl;
    }
}

int main() {
    HashSet s;
    string commandLine;

    while (true) {
        getline(cin, commandLine);
        if (commandLine == "EXIT") {
            break;
        }
        processCommand(s, commandLine);
    }

    return 0;
}
