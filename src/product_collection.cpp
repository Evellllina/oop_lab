#include "product_collection.h"

#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

ProductCollection::ProductCollection(const ProductCollection& other) {
    ProductCollection copy;
    copy.reserve(other.size_);
    for (std::size_t i = 0; i < other.size_; ++i) {
        copy.add(*other.data_[i]);
    }
    swap(copy);
}

ProductCollection::ProductCollection(ProductCollection&& other) noexcept {
    swap(other);
}

ProductCollection& ProductCollection::operator=(const ProductCollection& other) {
    if (this != &other) {
        ProductCollection copy(other);
        swap(copy);
    }
    return *this;
}

ProductCollection& ProductCollection::operator=(ProductCollection&& other) noexcept {
    if (this != &other) {
        ProductCollection moved(std::move(other));
        swap(moved);
    }
    return *this;
}

ProductCollection::~ProductCollection() {
    clear();
    delete[] data_;
}

void ProductCollection::reserve(std::size_t capacity) {
    if (capacity <= capacity_) {
        return;
    }
    if (capacity > std::numeric_limits<std::size_t>::max() / sizeof(BankProduct*)) {
        throw std::length_error("collection is too large");
    }
    auto next = new BankProduct*[capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        next[i] = data_[i];
    }
    delete[] data_;
    data_ = next;
    capacity_ = capacity;
}

void ProductCollection::add(const BankProduct& product) {
    std::unique_ptr<BankProduct> copy(product.clone());
    if (!copy) {
        throw std::logic_error("clone returned nullptr");
    }
    if (size_ == capacity_) {
        const auto limit = std::numeric_limits<std::size_t>::max() / sizeof(BankProduct*);
        if (capacity_ >= limit) {
            throw std::length_error("collection is too large");
        }
        reserve(capacity_ == 0 ? 1 : (capacity_ > limit / 2 ? limit : capacity_ * 2));
    }
    data_[size_++] = copy.release();
}

void ProductCollection::remove(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("product index is out of range");
    }
    delete data_[index];
    for (std::size_t i = index + 1; i < size_; ++i) {
        data_[i - 1] = data_[i];
    }
    data_[--size_] = nullptr;
}

void ProductCollection::clear() noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        delete data_[i];
        data_[i] = nullptr;
    }
    size_ = 0;
}

void ProductCollection::swap(ProductCollection& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

std::size_t ProductCollection::size() const noexcept {
    return size_;
}

bool ProductCollection::empty() const noexcept {
    return size_ == 0;
}

BankProduct& ProductCollection::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("product index is out of range");
    }
    return *data_[index];
}

const BankProduct& ProductCollection::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("product index is out of range");
    }
    return *data_[index];
}

double ProductCollection::totalMetric() const {
    long double total = 0.0L;
    for (std::size_t i = 0; i < size_; ++i) {
        total += data_[i]->metric();
    }
    if (total > std::numeric_limits<double>::max()) {
        throw std::overflow_error("total metric is outside the finite range");
    }
    return static_cast<double>(total);
}

void ProductCollection::sortByMetric() {
    for (std::size_t i = 1; i < size_; ++i) {
        std::size_t j = i;
        while (j > 0 && data_[j]->metric() < data_[j - 1]->metric()) {
            std::swap(data_[j], data_[j - 1]);
            --j;
        }
    }
}
