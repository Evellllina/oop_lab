#pragma once

#include "bank_product.h"

#include <cstddef>

class ProductCollection {
public:
    ProductCollection() = default;
    ProductCollection(const ProductCollection& other);
    ProductCollection(ProductCollection&& other) noexcept;
    ProductCollection& operator=(const ProductCollection& other);
    ProductCollection& operator=(ProductCollection&& other) noexcept;
    ~ProductCollection();

    void add(const BankProduct& product);
    void remove(std::size_t index);
    void clear() noexcept;
    void swap(ProductCollection& other) noexcept;
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    BankProduct& at(std::size_t index);
    const BankProduct& at(std::size_t index) const;
    double totalMetric() const;
    void sortByMetric();

private:
    BankProduct** data_ = nullptr;
    std::size_t size_ = 0;
    std::size_t capacity_ = 0;
    void reserve(std::size_t capacity);
};
