#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <limits>

using namespace std;

const int MAX_ALLOWED_VALUE = 2000000000;

// Узел для стека
struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

// Стек для значений и операторов
struct Stack {
    Node* top;

    Stack() : top(nullptr) {}

    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (top == nullptr) {
            throw runtime_error("Стек пуст, нечего удалять!");
        }
        Node* temp = top;
        top = top->next;
        delete temp;
    }

    int peek() const {
        if (top == nullptr) {
            throw runtime_error("Стек пуст!");
        }
        return top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    ~Stack() {
        clear();
    }

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

    void loadFromFile(const string &filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            while (inFile >> value) {
                push(value);
            }
            inFile.close();
            cout << "Стек загружен из файла " << filename << endl;
        } else {
            cout << "Не удалось открыть файл для чтения!" << endl;
        }
    }
};

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Функция вычисления операций с проверкой на большие числа
int applyOp(int a, int b, char op) {
    int result = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': 
            if (abs(a) > MAX_ALLOWED_VALUE / abs(b)) throw runtime_error("Результат превышает допустимое значение!");
            result = a * b;
            break;
        case '/':
            if (b == 0) throw runtime_error("Деление на ноль!");
            result = a / b;
            break;
    }
    if (abs(result) > MAX_ALLOWED_VALUE) throw runtime_error("Результат превышает допустимое значение!");
    return result;
}

// Функция вычисления выражения
int evaluateExpression(const string &expression) {
    Stack values;
    Stack ops;

    for (size_t i = 0; i < expression.length(); i++) {
        if (isspace(expression[i])) continue;

        if (isdigit(expression[i])) {
            int val = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                val = (val * 10) + (expression[i] - '0');
                i++;
            }
            if (val > MAX_ALLOWED_VALUE) throw runtime_error("Число превышает допустимое значение!");
            values.push(val);
            i--;
        } else if (expression[i] == '(') {
            ops.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!ops.isEmpty() && ops.peek() != '(') {
                int val2 = values.peek(); values.pop();
                int val1 = values.peek(); values.pop();
                char op = ops.peek(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
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
            break;
        }
        processCommand(stack, commandLine);
    }

    return 0;
}
