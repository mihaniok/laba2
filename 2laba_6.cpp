#include <iostream>
#include <string>
#include <sstream>

using namespace std;

const int capacity = 100;  // Максимальное количество сотрудников и менеджеров

// Узел для хранения подчинённого
struct Subordinate {
    string employee;
    Subordinate* next;
    Subordinate(const string& emp) : employee(emp), next(nullptr) {}
};

// Узел для хранения менеджера и списка его подчинённых
struct ManagerNode {
    string manager;
    Subordinate* subordinates;
    int subordinateCount;
    bool counted;  // Флаг для отметки, что подчинённые были посчитаны
    ManagerNode* next;
    ManagerNode(const string& mgr) : manager(mgr), subordinates(nullptr), subordinateCount(0), counted(false), next(nullptr) {}
};

// Таблица для хранения менеджеров
struct ManagerTable {
    ManagerNode* table[capacity];

    ManagerTable() {
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    ~ManagerTable() {
        for (int i = 0; i < capacity; ++i) {
            ManagerNode* current = table[i];
            while (current) {
                Subordinate* subCurrent = current->subordinates;
                while (subCurrent) {
                    Subordinate* toDelete = subCurrent;
                    subCurrent = subCurrent->next;
                    delete toDelete;
                }
                ManagerNode* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
    }
};

// Хеш-функция для получения индекса по ключу
int hashFunction(const string& key) {
    int sum = 0;
    for (char c : key) {
        sum += static_cast<int>(c);
    }
    return sum % capacity;
}

// Функция добавления подчинённого к менеджеру
void addSubordinate(ManagerTable& table, const string& employee, const string& manager) {
    if (employee == manager) {
        // Игнорируем пары, где сотрудник и менеджер одинаковы
        return;
    }

    int index = hashFunction(manager);
    ManagerNode* current = table.table[index];

    while (current && current->manager != manager) {
        current = current->next;
    }

    if (!current) {
        current = new ManagerNode(manager);
        current->next = table.table[index];
        table.table[index] = current;
    }

    Subordinate* newSubordinate = new Subordinate(employee);
    newSubordinate->next = current->subordinates;
    current->subordinates = newSubordinate;
    current->subordinateCount += 1;
}

// Рекурсивная функция для подсчёта всех подчинённых, включая косвенные
int countAllSubordinates(ManagerTable& table, const string& manager) {
    int index = hashFunction(manager);
    ManagerNode* current = table.table[index];

    while (current && current->manager != manager) {
        current = current->next;
    }

    if (!current) return 0;

    if (current->counted) return current->subordinateCount;

    Subordinate* subCurrent = current->subordinates;
    while (subCurrent) {
        current->subordinateCount += countAllSubordinates(table, subCurrent->employee);
        subCurrent = subCurrent->next;
    }

    current->counted = true;
    return current->subordinateCount;
}

// Вывод всех менеджеров и количества их подчинённых
void printHierarchy(ManagerTable& table) {
    for (int i = 0; i < capacity; ++i) {
        ManagerNode* current = table.table[i];
        while (current) {
            int totalSubordinates = countAllSubordinates(table, current->manager);
            cout << current->manager << ": " << totalSubordinates << endl;
            current = current->next;
        }
    }
}

int main() {
    ManagerTable table;
    string commandLine;

    while (true) {
        getline(cin, commandLine);
        if (commandLine == "PRINT") break;

        stringstream ss(commandLine);
        string employee, manager;
        ss >> employee >> manager;
        addSubordinate(table, employee, manager);
    }

    printHierarchy(table);

    return 0;
}
