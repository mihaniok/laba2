#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Set {
    int data[100];  // массив для хранения элементов множества
    int size;       // текущий размер множества

    Set() : size(0) {}

    // Добавление элемента с автоматической сортировкой
    void add(int value) {
        if (!contains(value)) {
            int i = size - 1;
            // Находим позицию для вставки, сдвигая элементы вправо
            while (i >= 0 && data[i] > value) {
                data[i + 1] = data[i];
                i--;
            }
            data[i + 1] = value;
            size++;
        }
    }

    // Удаление элемента
    void remove(int value) {
        int pos = -1;
        for (int i = 0; i < size; i++) {
            if (data[i] == value) {
                pos = i;
                break;
            }
        }
        if (pos != -1) {
            for (int i = pos; i < size - 1; i++) {
                data[i] = data[i + 1];
            }
            size--;
        }
    }

    // Проверка наличия элемента
    bool contains(int value) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == value) {
                return true;
            }
        }
        return false;
    }

    // Вывод множества
    void print() const {
        cout << "Set: ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    // Запись множества в файл
    void saveToFile() const {
        ofstream outFile("set_data.txt");
        if (outFile.is_open()) {
            for (int i = 0; i < size; i++) {
                outFile << data[i] << " ";
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
            size = 0;  // сбрасываем размер перед загрузкой
            int value;
            while (inFile >> value) {
                add(value);  // добавляем элементы с сортировкой
            }
            inFile.close();
            cout << "Set loaded from file." << endl;
        } else {
            cout << "Error opening file for reading!" << endl;
        }
    }
};

// Обработка команд
void processCommand(Set &s, const string &commandLine) {
    if (commandLine.find("SETADD") == 0) {
        int value = stoi(commandLine.substr(7));
        s.add(value);
    } else if (commandLine.find("SETDEL") == 0) {
        int value = stoi(commandLine.substr(7));
        s.remove(value);
    } else if (commandLine.find("SET_AT") == 0) {
        int value = stoi(commandLine.substr(9));
        cout << (s.contains(value) ? "Yes" : "No") << endl;
    } else if (commandLine == "PRINT") {
        s.print();
    } else if (commandLine == "SAVE") {
        s.saveToFile();
    } else if (commandLine == "LOAD") {
        s.loadFromFile();
    }
}

int main() {
    Set s;
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
