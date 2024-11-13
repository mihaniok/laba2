#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Set {
    int data[100];  // Массив для хранения элементов множества
    int size;       // Текущий размер множества

    Set() : size(0) {}

    // Добавление элемента с автоматической сортировкой
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

    // Проверка наличия элемента (быстрый поиск в отсортированном массиве)
    bool contains(int value) const {
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (data[mid] == value) return true;
            else if (data[mid] < value) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }

    // Подсчет общих элементов с другим множеством
    int countIntersection(const Set& other) const {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (other.contains(data[i])) {
                count++;
            }
        }
        return count;
    }

    // Вывод множества
    void print() const {
        cout << "{ ";
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << "}";
    }
};

// Функция для нахождения пары множеств с максимальным количеством общих элементов
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

    // Вывод результата
    if (set1Index != -1 && set2Index != -1) {
        cout << "Максимальное количество общих элементов: " << maxIntersection << endl;
        cout << "Множества с максимальным пересечением: ";
        sets[set1Index].print();
        cout << " и ";
        sets[set2Index].print();
        cout << endl;
    } else {
        cout << "Пересечений между множествами нет." << endl;
    }
}

int main() {
    const int MAX_SETS = 10;
    Set sets[MAX_SETS];
    int setCount = 0;

    cout << "Введите количество множеств: ";
    cin >> setCount;
    cin.ignore(); // игнорируем перевод строки после ввода количества множеств
    if (setCount > MAX_SETS) setCount = MAX_SETS;

    // Ввод множеств
    for (int i = 0; i < setCount; i++) {
        cout << "Введите элементы для множества #" << i + 1 << ": ";
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

    // Нахождение и вывод пары с максимальным пересечением
    findMaxIntersection(sets, setCount);

    return 0;
}
