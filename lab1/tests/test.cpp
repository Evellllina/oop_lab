#include <gtest/gtest.h>
#include "array_ops.h"
#include "variant_5.h"

// Создание пустого массива
TEST(ArrayOpsTest, CreateEmptyArray) {
    int* arr = array_create(0);
    EXPECT_EQ(arr, nullptr);
    
    array_delete(arr);
    EXPECT_EQ(arr, nullptr);
}

//Вставка элемента в начало
TEST(ArrayOpsTest, InsertAtBeginning) {
    int* arr = array_create(3);
    std::size_t size = 3;
    arr[0] = 2;
    arr[1] = 3;
    arr[2] = 4;
    
    arr = array_insert(arr, size, 0, 1);
    
    EXPECT_EQ(size, 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    
    array_delete(arr);
}

//Вставка элемента в середину
TEST(ArrayOpsTest, InsertInMiddle) {
    int* arr = array_create(3);
    std::size_t size = 3;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 4;
    
    arr = array_insert(arr, size, 2, 3);
    
    EXPECT_EQ(size, 4);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    
    array_delete(arr);
}

//Удаление элемента
TEST(ArrayOpsTest, RemoveElement) {
    int* arr = array_create(4);
    std::size_t size = 4;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    
    arr = array_remove(arr, size, 1); 
    
    EXPECT_EQ(size, 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);
    EXPECT_EQ(arr[2], 4);
    
    array_delete(arr);
}

//Изменение размера (увеличение)
TEST(ArrayOpsTest, ResizeIncrease) {
    int* arr = array_create(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    
    int* new_arr = array_resize(arr, 3, 5);
    
    EXPECT_EQ(new_arr[0], 1);
    EXPECT_EQ(new_arr[1], 2);
    EXPECT_EQ(new_arr[2], 3);
    EXPECT_EQ(new_arr[3], 0);
    EXPECT_EQ(new_arr[4], 0);
    
    array_delete(new_arr);
}

//Изменение размера (уменьшение)
TEST(ArrayOpsTest, ResizeDecrease) {
    int* arr = array_create(5);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;
    
    int* new_arr = array_resize(arr, 5, 3);
    
    EXPECT_EQ(new_arr[0], 1);
    EXPECT_EQ(new_arr[1], 2);
    EXPECT_EQ(new_arr[2], 3);
    
    array_delete(new_arr);
}

//Сортировка подсчётом
TEST(Variant5Test, CountingSortBasic) {
    int arr[] = {5, 2, 8, 2, 9, 1, 5};
    std::size_t size = 7;
    
    array_counting_sort(arr, size);
    
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 2);
    EXPECT_EQ(arr[3], 5);
    EXPECT_EQ(arr[4], 5);
    EXPECT_EQ(arr[5], 8);
    EXPECT_EQ(arr[6], 9);
}

//Сортировка подсчётом - пустой массив
TEST(Variant5Test, CountingSortEmpty) {
    int* arr = nullptr;
    array_counting_sort(arr, 0);
    EXPECT_TRUE(true);
}

//Сортировка подсчётом - массив из одного элемента
TEST(Variant5Test, CountingSortSingleElement) {
    int arr[] = {42};
    std::size_t size = 1;
    
    array_counting_sort(arr, size);
    
    EXPECT_EQ(arr[0], 42);
}

//Сортировка подсчётом - все элементы одинаковые
TEST(Variant5Test, CountingSortAllSame) {
    int arr[] = {5, 5, 5, 5};
    std::size_t size = 4;
    
    array_counting_sort(arr, size);
    
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 5);
    EXPECT_EQ(arr[2], 5);
    EXPECT_EQ(arr[3], 5);
}

//Подсчёт в диапазоне
TEST(Variant5Test, RangeCountBasic) {
    int arr[] = {3, 7, 3, 9, 5, 2};
    std::size_t size = 6;
    std::size_t count = array_range_count(arr, size, 3, 6);
    EXPECT_EQ(count, 3);
}

//Подсчёт в диапазоне - нет совпадений
TEST(Variant5Test, RangeCountNoMatches) {
    int arr[] = {1, 2, 3};
    std::size_t size = 3;
    std::size_t count = array_range_count(arr, size, 10, 20);
    EXPECT_EQ(count, 0);
}

// Подсчёт в диапазоне - все элементы подходят
TEST(Variant5Test, RangeCountAllMatch) {
    int arr[] = {5, 6, 7, 8};
    std::size_t size = 4;
    std::size_t count = array_range_count(arr, size, 0, 10);
    EXPECT_EQ(count, 4);
}

//Подсчёт в диапазоне - пустой массив
TEST(Variant5Test, RangeCountEmpty) {
    std::size_t count = array_range_count(nullptr, 0, 1, 10);
    EXPECT_EQ(count, 0);
}


//Граничные значения (0 и 1023)
TEST(Variant5Test, CountingSortBoundaryValues) {
    int arr[] = {1023, 0, 512, 0, 1023};
    std::size_t size = 5;
    array_counting_sort(arr, size);
    EXPECT_EQ(arr[0], 0);
    EXPECT_EQ(arr[1], 0);
    EXPECT_EQ(arr[2], 512);
    EXPECT_EQ(arr[3], 1023);
    EXPECT_EQ(arr[4], 1023);
}