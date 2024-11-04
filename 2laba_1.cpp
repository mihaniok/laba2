#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>

using namespace std;

// Узел для стека
struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

// Стек для значений и операторов
struct Stack {
    Node* top;  // Указатель на вершину стека

    Stack() : top(nullptr) {}

    // Добавление элемента на вершину стека
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    // Удаление элемента с вершины стека
    void pop() {
        if (top == nullptr) {
            throw runtime_error("Стек пуст, нечего удалять!");
        }
        Node* temp = top;
        top = top->next;
        delete temp;
    }

    // Получение значения с вершины стека
    int peek() const {
        if (top == nullptr) {
            throw runtime_error("Стек пуст!");
        }
        return top->data;
    }

    // Проверка пустоты стека
    bool isEmpty() const {
        return top == nullptr;
    }

    // Очистка стека
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Деструктор
    ~Stack() {
        clear();  // Освобождаем память при удалении стека
    }

    // Вывод всех элементов стека
    void print() const {
        if (top == nullptr) {
            cout << "Стек пуст!" << endl;
            return;
        }
        Node* temp = top;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    // Запись стека в файл
    void saveToFile(const string &filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            Node* temp = top;
            while (temp != nullptr) {
                outFile << temp->data << " ";
                temp = temp->next;
            }
            outFile.close();
            cout << "Стек сохранён в файл " << filename << endl;
        } else {
            cout << "Не удалось открыть файл для записи!" << endl;
        }
    }

    // Загрузка стека из файла
    void loadFromFile(const string &filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            while (inFile >> value) {
                push(value);  // добавляем элементы в стек
            }
            inFile.close();
            cout << "Стек загружен из файла " << filename << endl;
        } else {
            cout << "Не удалось открыть файл для чтения!" << endl;
        }
    }
};

// Определение приоритета операций
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Функция вычисления выражения
int applyOp(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("Деление на ноль!");
            return a / b;
    }
    return 0;
}

// Функция вычисления выражения
int evaluateExpression(const string &expression) {
    Stack values; // Стек для значений
    Stack ops;    // Стек для операторов

    for (size_t i = 0; i < expression.length(); i++) {
        if (isspace(expression[i])) {
            continue; // Игнорируем пробелы
        }

        if (isdigit(expression[i])) {
            int val = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                val = (val * 10) + (expression[i] - '0');
                i++;
            }
            values.push(val);
            i--; // Уменьшаем i, чтобы не пропустить следующий символ
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.isEmpty() && ops.peek() != '(') {
                int val2 = values.peek(); values.pop();
                int val1 = values.peek(); values.pop();
                char op = ops.peek(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.pop(); // Удаляем '('
        } else {
            while (!ops.isEmpty() && precedence(ops.peek()) >= precedence(expression[i])) {
                int val2 = values.peek(); values.pop();
                int val1 = values.peek(); values.pop();
                char op = ops.peek(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expression[i]);
        }
    }

    while (!ops.isEmpty()) {
        int val2 = values.peek(); values.pop();
        int val1 = values.peek(); values.pop();
        char op = ops.peek(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.peek();
}

// Функция обработки команд
void processCommand(Stack &stack, const string &commandLine) {
    stringstream ss(commandLine);
    string command;
    ss >> command;

    if (command == "PUSH") {
        int value;
        ss >> value;
        stack.push(value);
    } else if (command == "POP") {
        stack.pop();
    } else if (command == "PRINT") {
        stack.print();
    } else if (command == "SAVE") {
        string filename;
        ss >> filename;
        stack.saveToFile(filename);
    } else if (command == "LOAD") {
        string filename;
        ss >> filename;
        stack.loadFromFile(filename);
    } else if (command == "SOLVE") {
        string expression;
        getline(ss, expression);
        try {
            int result = evaluateExpression(expression);
            cout << "Результат: " << result << endl;
        } catch (const runtime_error &e) {
            cout << "Ошибка: " << e.what() << endl;
        }
    }
}

int main() {
    Stack stack;
    string commandLine;

    while (true) {
        getline(cin, commandLine);

        if (commandLine == "EXIT") {
            break;  // выход из программы
        }

        processCommand(stack, commandLine);  // обработка команды
    }

    return 0;
}

/*
PUSH <value> — добавляет элемент на вершину стека.
POP — удаляет элемент с вершины стека.
PRINT — выводит все элементы стека.
SAVE <filename> — сохраняет стек в файл.
LOAD <filename> — загружает стек из файла.
SOLVE <expression> — вычисляет числовое выражение.
EXIT — завершает выполнение программы.
*/
