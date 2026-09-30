#include <iostream>
#include <cstdlib>
#include <ctime>
#include "array_ops.h"
#include "variant_5.h"

bool read_int(const char* prompt, int& value) {
    std::cout << prompt;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

bool read_size_t(const char* prompt, std::size_t& value) {
    std::cout << prompt;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int* arr = nullptr;
    std::size_t size = 0;
    int choice = -1;

    do {
        std::cout << "1. Создать массив\n";
        std::cout << "2. Напечатать\n";
        std::cout << "3. Вставить элемент\n";
        std::cout << "4. Удалить элемент\n";
        std::cout << "5. Изменить размер\n";
        std::cout << "6. Печать\n";
        std::cout << "7. Алгоритм варианта (Сортировка + Диапазон)\n";
        std::cout << "0. Выход\n";

        if (!read_int("Выберите действие: ", choice)) {
            std::cout << "Ошибка ввода\n";
            continue;
        }

        switch (choice) {
            case 1: { 
                std::size_t n;
                if (!read_size_t("Введите размер массива: ", n)) break;

                array_delete(arr); 

                arr = array_create(n);
                size = n;

                for (std::size_t i = 0; i < size; ++i) {
                    arr[i] = std::rand() % 1024;
                }
                std::cout << "Массив создан (" << size << " элементов).\n";
                break;
            }

            case 2: 
            case 6:
                if (arr == nullptr || size == 0) {
                    std::cout << "Массив пуст или не создан.\n";
                } else {
                    std::cout << "Текущий массив (размер " << size << "): ";
                    array_print(arr, size);
                }
                break;

            case 3: {
                if (arr == nullptr) {
                    std::cout << "Сначала создайте массив!\n";
                    break;
                }
                std::size_t pos;
                int val;
                std::cout << "Введите позицию для вставки (0.." << size << "): ";
                if (!(std::cin >> pos)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Ошибка ввода позиции.\n";
                    break;
                }

                std::cout << "Введите значение: ";
                if (!(std::cin >> val)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Ошибка ввода значения.\n";
                    break;
                }

                arr = array_insert(arr, size, pos, val);


                break;
            }

            case 4: { 
                if (arr == nullptr || size == 0) {
                    std::cout << "Массив пуст или не создан.\n";
                    break;
                }
                std::size_t pos;
                std::cout << "Введите позицию для удаления (0.." << size - 1 << "): ";
                if (!(std::cin >> pos)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Ошибка ввода позиции.\n";
                    break;
                }

                arr = array_remove(arr, size, pos);
                break;
            }

            case 5: {
                if (arr == nullptr) {
                    std::cout << "Сначала создайте массив\n";
                    break;
                }
                std::size_t new_size;
                std::cout << "Введите новый размер: ";
                if (!(std::cin >> new_size)) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    std::cout << "Ошибка ввода размера\n";
                    break;
                }


                arr = array_resize(arr, size, new_size);
                size = new_size;
                std::cout << "Размер изменён на " << size << ".\n";
                break;
            }

            case 7: {
                if (arr == nullptr || size == 0) {
                    std::cout << "Массив пуст или не создан.\n";
                    break;
                }

                std::cout << "Сортировка подсчётом\n";
                array_counting_sort(arr, size);
                std::cout << "Отсортированный массив: ";
                array_print(arr, size);

                int lo, hi;
                std::cout << "Подсчёт в диапазоне [lo, hi]\n";
                if (!read_int("Введите lo: ", lo)) break;
                if (!read_int("Введите hi: ", hi)) break;

                if (lo > hi) {
                    std::swap(lo, hi);
                    std::cout << "(Границы автоматически скорректированы)\n";
                }

                std::size_t count = array_range_count(arr, size, lo, hi);
                std::cout << "Количество элементов в [" << lo << ", " << hi << "]: "
                          << count << std::endl;
                break;
            }

            case 0:
                std::cout << "Выход из программы\n";
                break;

            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 0);

    array_delete(arr);

    return 0;
}


// cd ~/oop/lab1/build
// cmake --build .
// ./main_app
// ./unit_tests