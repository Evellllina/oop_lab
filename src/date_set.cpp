#include "date_set.h"
#include <cassert>
#include <utility>


DateSet::DateSet() : data_(nullptr), size_(0), capacity_(0) {}

DateSet::DateSet(const DateSet& other)
    : data_(other.size_ > 0 ? new Date[other.size_] : nullptr),
      size_(other.size_),
      capacity_(other.size_) {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

DateSet::DateSet(DateSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

DateSet& DateSet::operator=(const DateSet& other) {
    if (this != &other) {
        DateSet tmp(other);
        swap(tmp);
    }
    return *this;
}

DateSet& DateSet::operator=(DateSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

DateSet::~DateSet() {
    delete[] data_;
}

void DateSet::swap(DateSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}


std::size_t DateSet::lower_bound(const Date& d) const {
    std::size_t lo = 0;
    std::size_t hi = size_;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (data_[mid] < d) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

void DateSet::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) return;
    Date* fresh = new Date[new_capacity];
    for (std::size_t i = 0; i < size_; ++i) {
        fresh[i] = data_[i];
    }
    delete[] data_;
    data_ = fresh;
    capacity_ = new_capacity;
}

void DateSet::push_back_sorted(const Date& d) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_++] = d;
}


bool DateSet::add(const Date& d) {
    if (!d.is_valid()) return false;
    std::size_t pos = lower_bound(d);
    if (pos < size_ && data_[pos] == d) return false;

    Date value = d;
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    for (std::size_t i = size_; i > pos; --i) {
        data_[i] = data_[i - 1];
    }
    data_[pos] = value;
    ++size_;
    return true;
}

bool DateSet::remove(const Date& d) {
    std::size_t pos = lower_bound(d);
    if (pos >= size_ || data_[pos] != d) return false;
    for (std::size_t i = pos; i + 1 < size_; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
    return true;
}

bool DateSet::contains(const Date& d) const {
    std::size_t pos = lower_bound(d);
    return pos < size_ && data_[pos] == d;
}

bool DateSet::operator()(const Date& d) const {
    return contains(d);
}

std::size_t DateSet::size() const {
    return size_;
}

bool DateSet::empty() const {
    return size_ == 0;
}

std::size_t DateSet::capacity() const {
    return capacity_;
}

const Date& DateSet::operator[](std::size_t i) const {
    assert(i < size_);
    return data_[i];
}

DateSet& DateSet::operator+=(const Date& d) {
    add(d);
    return *this;
}

DateSet& DateSet::operator-=(const Date& d) {
    remove(d);
    return *this;
}


DateSet DateSet::operator|(const DateSet& other) const {
    DateSet result;
    result.reserve(size_ + other.size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            result.push_back_sorted(data_[i++]);
        } else if (other.data_[j] < data_[i]) {
            result.push_back_sorted(other.data_[j++]);
        } else {
            result.push_back_sorted(data_[i]);
            ++i;
            ++j;
        }
    }
    while (i < size_) result.push_back_sorted(data_[i++]);
    while (j < other.size_) result.push_back_sorted(other.data_[j++]);
    return result;
}

DateSet DateSet::operator&(const DateSet& other) const {
    DateSet result;
    result.reserve(size_ < other.size_ ? size_ : other.size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            ++i;
        } else if (other.data_[j] < data_[i]) {
            ++j;
        } else {
            result.push_back_sorted(data_[i]);
            ++i;
            ++j;
        }
    }
    return result;
}

DateSet DateSet::operator-(const DateSet& other) const {
    DateSet result;
    result.reserve(size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            result.push_back_sorted(data_[i++]);
        } else if (other.data_[j] < data_[i]) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }
    while (i < size_) result.push_back_sorted(data_[i++]);
    return result;
}

bool DateSet::operator==(const DateSet& other) const {
    if (size_ != other.size_) return false;
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] != other.data_[j]) return false;
        ++i;
        ++j;
    }
    return true;
}

bool DateSet::operator!=(const DateSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const DateSet& s) {
    os << "{ ";
    for (std::size_t i = 0; i < s.size_; ++i) {
        if (i > 0) os << ", ";
        os << s.data_[i];
    }
    if (s.size_ > 0) os << ' ';
    os << '}';
    return os;
}
