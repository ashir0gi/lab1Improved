#include <iostream>
#include <random>
#include <chrono>
#include "MyPtrLib.h"
struct TestData {
    int data;
};

int main() {
    const int numObjects = 1000000; // Количество объектов для создания и обработки

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    // Измерение времени для обычных указателей
    auto startTime = std::chrono::high_resolution_clock::now();

    // Создание объектов с обычными указателями и заполнение случайными данными
    TestData** rawPointers = new TestData*[numObjects];
    for (int i = 0; i < numObjects; ++i) {
        rawPointers[i] = new TestData();
        rawPointers[i]->data = dist(rng); // Заполнение случайными данными
    }

    // Освобождение памяти, замер времени и удаление указателей
    for (int i = 0; i < numObjects; ++i) {
        delete rawPointers[i];
    }
    delete[] rawPointers;

    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTimeRawPtr = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Time taken with raw pointers: " << elapsedTimeRawPtr.count() << " ms" << std::endl;

    // Измерение времени для UniquePtr
    startTime = std::chrono::high_resolution_clock::now();

    // Создание объектов с использованием UniquePtr и заполнение случайными данными
    UniquePtr<TestData>* uniquePtrs = new UniquePtr<TestData>[numObjects];
    for (int i = 0; i < numObjects; ++i) {
        uniquePtrs[i] = UniquePtr<TestData>(new TestData());
        uniquePtrs[i]->data = dist(rng); // Заполнение случайными данными
    }

    // Замер времени (удаление указателей автоматически)

    endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTimeUniquePtr = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Time taken with UniquePtr: " << elapsedTimeUniquePtr.count() << " ms" << std::endl;

    delete[] uniquePtrs; // Освобождение памяти

    return 0;
}