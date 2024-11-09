#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

struct Set {
    int data[100];  // Array to store set elements
    int size;       // Current size of the set

    Set() : size(0) {}

    // Add an element with automatic sorting
    void add(int value) {
        if (!contains(value)) {
            int i = size - 1;
            while (i >= 0 && data[i] > value) {
                data[i + 1] = data[i];
                i--;
            }
            data[i + 1] = value;
            size++;
        }
    }

    // Check if an element exists
    bool contains(int value) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == value) {
                return true;
            }
        }
        return false;
    }

    // Count common elements with another set
    int countIntersection(const Set& other) const {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (other.contains(data[i])) {
                count++;
            }
        }
        return count;
    }

    // Print the set
    void print() const {
        cout << "{ ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << "}";
    }
};

// Function to find the pair of sets with the maximum number of common elements
void findMaxIntersection(Set sets[], int setCount) {
    int maxIntersection = 0;
    int set1Index = -1, set2Index = -1;

    for (int i = 0; i < setCount; i++) {
        for (int j = i + 1; j < setCount; j++) {
            int intersection = sets[i].countIntersection(sets[j]);
            if (intersection > maxIntersection) {
                maxIntersection = intersection;
                set1Index = i;
                set2Index = j;
            }
        }
    }

    // Output the result
    if (set1Index != -1 && set2Index != -1) {
        cout << "Maximum number of common elements: " << maxIntersection << endl;
        cout << "Sets with the maximum intersection: ";
        sets[set1Index].print();
        cout << " and ";
        sets[set2Index].print();
        cout << endl;
    } else {
        cout << "No intersections between sets." << endl;
    }
}

int main() {
    const int MAX_SETS = 10;
    Set sets[MAX_SETS];
    int setCount = 0;

    cout << "Enter the number of sets: ";
    cin >> setCount;
    cin.ignore(); // ignore newline after set count input
    if (setCount > MAX_SETS) setCount = MAX_SETS;

    // Enter the sets
    for (int i = 0; i < setCount; i++) {
        cout << "Enter elements for set #" << i + 1 << ": ";
        string line;
        while (true) {
            getline(cin, line);
            if (line.empty()) break;
            stringstream ss(line);
            int value;
            while (ss >> value) {
                sets[i].add(value);
            }
        }
    }

    // Find and display pairs with maximum intersection
    findMaxIntersection(sets, setCount);

    return 0;
}
