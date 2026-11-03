#include <iostream>
#include <utility>
#include "date.h"
#include "date_set.h"

int main() {
    DateSet a;
    a.add(Date(2024, 3, 15));
    a.add(Date(2023, 12, 31));
    a.add(Date(2024, 1, 1));
    std::cout << "a = " << a << '\n';

    std::cout << "add(2024-01-01) повторно: " << a.add(Date(2024, 1, 1)) << '\n';
    std::cout << "add(2023-02-29) невалидна: " << a.add(Date(2023, 2, 29)) << '\n';
    std::cout << "add(2024-02-29) високосный: " << a.add(Date(2024, 2, 29)) << '\n';
    std::cout << "a = " << a << '\n';
    DateSet b;
    b += Date(2024, 1, 1);
    b += Date(2024, 2, 29);
    b += Date(2025, 5, 9);
    std::cout << "b = " << b << '\n';

    std::cout << "a | b = " << (a | b) << '\n';
    std::cout << "a & b = " << (a & b) << '\n';
    std::cout << "a - b = " << (a - b) << '\n';

    std::cout << "a(2024-03-15) = " << a(Date(2024, 3, 15)) << '\n';
    a -= Date(2024, 3, 15);
    std::cout << "после a -= 2024-03-15: " << a << '\n';

    DateSet c(a);
    std::cout << "c == a: " << (c == a) << '\n';
    DateSet d(std::move(c));
    std::cout << "после перемещения: c = " << c << ", d = " << d << '\n';
    return 0;
}

//cmake --build build-asan
//./build-asan/lab_03_tests
//./build-asan/lab_03_main
