#include "array_ops.h"
#include <iostream>
#include <algorithm>

int* array_create(std::size_t size) { 
    if (size == 0) return nullptr;
    return new int[size](); 
}

void array_delete(int*& arr) {
    delete[] arr;
    arr = nullptr;
}

int* array_resize(int* arr, std::size_t old_size, std::size_t new_size) {
    if (new_size == 0) {
        array_delete(arr);
        return nullptr;
    }
    
    int* new_arr = new int[new_size]();
    std::size_t copy_size = std::min(old_size, new_size);
    
    for (std::size_t i = 0; i < copy_size; ++i) {
        new_arr[i] = arr[i];
    }
    
    delete[] arr;
    return new_arr;
}

int* array_insert(int* arr, std::size_t& size, std::size_t pos, int value) {
    if (pos > size) {
        std::cout << "Ошибка: позиция " << pos << " вне диапазона [0, " << size << "]\n";
        return arr;
    }
    
    arr = array_resize(arr, size, size + 1);
    
    for (std::size_t i = size; i > pos; --i) {
        arr[i] = arr[i - 1];
    }
    arr[pos] = value;
    size++;
    
    return arr;
}

int* array_remove(int* arr, std::size_t& size, std::size_t pos) {
    if (size == 0) {
        std::cout << "Ошибка: массив пуст\n";
        return arr;
    }
    
    if (pos >= size) {
        std::cout << "Ошибка: позиция " << pos << " вне диапазона [0, " << size - 1 << "]\n";
        return arr;
    }
    
    for (std::size_t i = pos; i < size - 1; ++i) {
        arr[i] = arr[i + 1];
    }
    
    arr = array_resize(arr, size, size - 1);
    size--;
    
    return arr;
}

void array_print(const int* arr, std::size_t size) {
    if (arr == nullptr || size == 0) {
        std::cout << "Пустой массив\n";
        return;
    }
    
    std::cout << "[";
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << arr[i];
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}