#include "card.h"
#include "credit.h"
#include "deposit.h"
#include "product_collection.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

void printCollection(const ProductCollection& products) {
    for (std::size_t i = 0; i < products.size(); ++i) {
        const BankProduct& product = products.at(i);
        std::cout << i << ": " << product;
        if (const auto* deposit = dynamic_cast<const Deposit*>(&product)) {
            std::cout << " isCumulative=" << deposit->isCumulative();
        } else if (const auto* credit = dynamic_cast<const Credit*>(&product)) {
            std::cout << " isAnnuity=" << credit->isAnnuity();
        } else if (const auto* card = dynamic_cast<const Card*>(&product)) {
            std::cout << " hasCashback=" << card->hasCashback();
        }
        std::cout << '\n';
    }
    std::cout << "Size: " << products.size() << "\nTotal metric: " << products.totalMetric() << '\n';
}

void requireEnd(std::istream& in) {
    if (in.fail()) {
        throw std::invalid_argument("invalid arguments");
    }
    if (in.eof()) {
        return;
    }
    in >> std::ws;
    if (!in.eof()) {
        throw std::invalid_argument("unexpected extra arguments");
    }
}

template<class Product>
void readAndAdd(std::istream& in, ProductCollection& products) {
    Product product;
    in >> product;
    requireEnd(in);
    products.add(product);
}

void help() {
    std::cout << "deposit sum rate years cumulative\n"
                 "credit sum rate years annuity\n"
                 "card spending rate\n"
                 "list | total | sort | remove index | clear | help | quit\n"
                 "Flags: 0 = false, nonzero integer = true. Indices start at 0.\n"
                 "Rates follow the assignment: fractions except annuity credit (percent).\n";
}

void demo() {
    ProductCollection products;
    products.add(Deposit(100000.0, 0.1, 2.0, true));
    products.add(Credit(120000.0, 12.0, 1.0, true));
    products.add(Card(50000.0, 0.02));
    products.add(Deposit(100000.0, 0.08, 1.0, false));
    std::cout << "Before sorting:\n";
    printCollection(products);
    products.sortByMetric();
    std::cout << "\nAfter sorting by metric (ascending):\n";
    printCollection(products);
    const BankProduct& first = products.at(0);
    std::cout << "First metric as double: " << static_cast<double>(first) << '\n';
    const Card sameMetric(100000.0, 0.01);
    std::cout << "First product equals Card(100000, 0.01) by metric: "
              << (first == sameMetric) << '\n';
}

void interactive() {
    ProductCollection products;
    help();
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        std::istringstream in(line);
        std::string command;
        if (!(in >> command)) {
            continue;
        }
        try {
            if (command == "deposit") {
                readAndAdd<Deposit>(in, products);
            } else if (command == "credit") {
                readAndAdd<Credit>(in, products);
            } else if (command == "card") {
                readAndAdd<Card>(in, products);
            } else if (command == "remove") {
                long long index = -1;
                in >> index;
                requireEnd(in);
                if (index < 0) {
                    throw std::out_of_range("index must be nonnegative");
                }
                products.remove(static_cast<std::size_t>(index));
            } else if (command == "list") {
                requireEnd(in);
                printCollection(products);
            } else if (command == "total") {
                requireEnd(in);
                std::cout << products.totalMetric() << '\n';
            } else if (command == "sort") {
                requireEnd(in);
                products.sortByMetric();
                printCollection(products);
            } else if (command == "clear") {
                requireEnd(in);
                products.clear();
            } else if (command == "help") {
                requireEnd(in);
                help();
            } else if (command == "quit") {
                requireEnd(in);
                return;
            } else {
                throw std::invalid_argument("unknown command; enter help");
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << '\n';
        }
    }
}

}

int main(int argc, char* argv[]) {
    try {
        if (argc == 1 || (argc == 2 && std::string(argv[1]) == "--demo")) {
            demo();
        } else if (argc == 2 && std::string(argv[1]) == "--interactive") {
            interactive();
        } else {
            std::cerr << "Usage: lab_04_main [--demo|--interactive]\n";
            return 1;
        }
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}

// ctest --test-dir build --output-on-failure
// ./build/lab_04_main
// ./build/lab_04_main --interactive