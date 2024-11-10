#include <iostream>
#include <sstream>

using namespace std;

class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    Node* root;

    Node* insert(Node* node, int value, int depth) {
        if (!node) {
            cout << depth << " ";  // Выводим глубину нового элемента
            return new Node(value);
        }
        if (value < node->value) {
            node->left = insert(node->left, value, depth + 1);
        } else if (value > node->value) {
            node->right = insert(node->right, value, depth + 1);
        }
        return node;
    }

    // Проверка на наличие элемента в дереве
    bool contains(Node* node, int value) {
        if (!node) return false;
        if (node->value == value) return true;
        if (value < node->value) return contains(node->left, value);
        return contains(node->right, value);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(int value) {
        // Если элемента нет в дереве, добавляем его
        if (!contains(root, value)) {
            root = insert(root, value, 1);
        }
    }
};

int main() {
    BinarySearchTree bst;

    string input;
    getline(cin, input);
    istringstream iss(input);

    int value;
    while (iss >> value) {
        bst.insert(value);
    }

    cout << endl;
    return 0;
}
