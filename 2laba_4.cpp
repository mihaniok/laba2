#include <iostream>

using namespace std;

struct Block {
    int width;
    int height;
};

// Функция сортировки блоков по убыванию ширины и высоты
void bubbleSort(Block blocks[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (blocks[j].width < blocks[j + 1].width || 
               (blocks[j].width == blocks[j + 1].width && blocks[j].height < blocks[j + 1].height)) {
                // Обмен элементов
                Block temp = blocks[j];
                blocks[j] = blocks[j + 1];
                blocks[j + 1] = temp;
            }
        }
    }
}

// Основная функция для построения пирамиды
int maxPyramidHeight(Block blocks[], int n) {
    bubbleSort(blocks, n);

    int maxHeight = 0;
    int lastWidth = blocks[0].width;
    maxHeight += blocks[0].height;

    for (int i = 1; i < n; i++) {
        if (blocks[i].width < lastWidth) {  // Только если ширина уменьшилась
            maxHeight += blocks[i].height;
            lastWidth = blocks[i].width;  // Обновляем последнюю используемую ширину
        }
    }

    return maxHeight;
}

int main() {
    int n;
    cout << "Введите количество блоков: ";
    cin >> n;

    Block* blocks = new Block[n];
    cout << "Введите ширину и высоту каждого блока:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> blocks[i].width >> blocks[i].height;
    }

    int result = maxPyramidHeight(blocks, n);
    cout << "Максимальная высота пирамиды: " << result << endl;

    delete[] blocks;
    return 0;
}
