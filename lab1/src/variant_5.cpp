#include "variant_5.h"

void array_counting_sort(int* arr, std::size_t size) {
    if (arr == nullptr || size == 0) return;
    
    const int MAX_VALUE = 1024;
    int* count = new int[MAX_VALUE]();
    
    for (std::size_t i = 0; i < size; ++i) {
        if (arr[i] >= 0 && arr[i] < MAX_VALUE) {
            count[arr[i]]++;
        }
    }

    std::size_t index = 0;
    for (int i = 0; i < MAX_VALUE; ++i) {
        while (count[i] > 0) {
            arr[index++] = i;
            count[i]--;
        }
    }
    
    delete[] count;
}

std::size_t array_range_count(const int* arr, std::size_t size, int lo, int hi) {
    if (arr == nullptr || size == 0) return 0;
    
    std::size_t count = 0;
    for (std::size_t i = 0; i < size; ++i) {
        if (arr[i] >= lo && arr[i] <= hi) {
            count++;
        }
    }
    
    return count;
}