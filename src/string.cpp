#include "string.h"

#include <cstring>
#include <stdexcept>
#include <utility>

namespace {

bool is_digit(char c) { return c >= '0' && c <= '9'; }
size_t digits_count(size_t n) {
    size_t d = 0;
    do {
        ++d;
        n /= 10;
    } while (n != 0);
    return d;
}

}  

String::String() : data_(new char[2]), size_(0), capacity_(1) {
    data_[0] = '\0';
}

String::String(const char* str) : data_(nullptr), size_(0), capacity_(1) {
    if (str != nullptr) {
        size_ = std::strlen(str);
    }
    if (size_ > capacity_) {
        capacity_ = size_;
    }
    data_ = new char[capacity_ + 1];
    if (size_ > 0) {
        std::memcpy(data_, str, size_);
    }
    data_[size_] = '\0';
}

String::String(const String& other)
    : data_(new char[other.capacity_ + 1]), size_(other.size_), capacity_(other.capacity_) {
    std::memcpy(data_, other.data_, size_ + 1);
}

String::String(String&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

String& String::operator=(String other) {
    swap(other);
    return *this;
}

String::~String() {
    delete[] data_;
}

void String::swap(String& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}


const char* String::c_str() const noexcept {
    return data_ != nullptr ? data_ : "";
}

char& String::operator[](size_t i) { return data_[i]; }
const char& String::operator[](size_t i) const { return data_[i]; }

char& String::at(size_t i) {
    if (i >= size_) throw std::out_of_range("String::at: index out of range");
    return data_[i];
}

const char& String::at(size_t i) const {
    if (i >= size_) throw std::out_of_range("String::at: index out of range");
    return data_[i];
}


void String::reserve(size_t new_capacity) {
    if (new_capacity <= capacity_ && data_ != nullptr) return;
    if (new_capacity < 1) new_capacity = 1;
    char* fresh = new char[new_capacity + 1];
    if (data_ != nullptr) {
        std::memcpy(fresh, data_, size_);
    }
    fresh[size_] = '\0';
    delete[] data_;
    data_ = fresh;
    capacity_ = new_capacity;
}

void String::grow_for(size_t needed) {
    if (needed <= capacity_ && data_ != nullptr) return;
    size_t cap = capacity_ == 0 ? 1 : capacity_;
    while (cap < needed) cap *= 2;
    reserve(cap);
}

void String::push_back(char c) {
    if (size_ == capacity_ || data_ == nullptr) {
        grow_for(size_ + 1);
    }
    data_[size_++] = c;
    data_[size_] = '\0';
}

void String::append(const char* str) {
    if (str == nullptr) return;
    size_t len = std::strlen(str);
    if (len == 0) return;
    if (size_ + len > capacity_ || data_ == nullptr) {
        String tmp(str);
        grow_for(size_ + len);
        std::memcpy(data_ + size_, tmp.data_, len);
    } else {
        std::memmove(data_ + size_, str, len);
    }
    size_ += len;
    data_[size_] = '\0';
}

void String::append(const String& other) {
    if (other.size_ == 0) return;
    if (this == &other) {
        String copy(other);
        append(copy);
        return;
    }
    grow_for(size_ + other.size_);
    std::memcpy(data_ + size_, other.data_, other.size_);
    size_ += other.size_;
    data_[size_] = '\0';
}

void String::clear() noexcept {
    size_ = 0;
    if (data_ != nullptr) data_[0] = '\0';
}

size_t String::find(const char* substr, size_t from) const {
    if (substr == nullptr) return npos;
    size_t len = std::strlen(substr);
    if (from > size_) return npos;
    if (len == 0) return from;
    if (len > size_ - from) return npos;
    for (size_t i = from; i + len <= size_; ++i) {
        if (std::memcmp(data_ + i, substr, len) == 0) return i;
    }
    return npos;
}

size_t String::find(const String& substr, size_t from) const {
    return find(substr.c_str(), from);
}

void String::print(std::ostream& os) const {
    os << c_str();
}

bool String::operator==(const String& other) const noexcept {
    if (size_ != other.size_) return false;
    return size_ == 0 || std::memcmp(data_, other.data_, size_) == 0;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    s.print(os);
    return os;
}

String String::rle_compress() const {
    String result;
    size_t i = 0;
    while (i < size_) {
        char c = data_[i];
        if (is_digit(c)) {
            throw std::invalid_argument("rle_compress: digits are not allowed in input");
        }
        size_t run = 1;
        while (i + run < size_ && data_[i + run] == c) ++run;

        result.push_back(c);
        char buf[24];
        size_t n = digits_count(run);
        buf[n] = '\0';
        size_t value = run;
        for (size_t k = n; k > 0; --k) {
            buf[k - 1] = static_cast<char>('0' + value % 10);
            value /= 10;
        }
        result.append(buf);

        i += run;
    }
    return result;
}

void String::rle_decompress() {
    size_t total = 0;
    size_t i = 0;
    while (i < size_) {
        char c = data_[i];
        if (is_digit(c)) {
            throw std::invalid_argument("rle_decompress: expected a symbol, got a digit");
        }
        ++i;
        if (i >= size_ || !is_digit(data_[i])) {
            throw std::invalid_argument("rle_decompress: symbol without a counter");
        }
        size_t count = 0;
        while (i < size_ && is_digit(data_[i])) {
            size_t d = static_cast<size_t>(data_[i] - '0');
            if (count > (npos - d) / 10) {
                throw std::invalid_argument("rle_decompress: counter overflow");
            }
            count = count * 10 + d;
            ++i;
        }
        if (count == 0) {
            throw std::invalid_argument("rle_decompress: counter must be >= 1");
        }
        if (total > npos - 1 - count) {
            throw std::invalid_argument("rle_decompress: result too large");
        }
        total += count;
    }

    String result;
    result.reserve(total);
    i = 0;
    while (i < size_) {
        char c = data_[i++];
        size_t count = 0;
        while (i < size_ && is_digit(data_[i])) {
            count = count * 10 + static_cast<size_t>(data_[i] - '0');
            ++i;
        }
        std::memset(result.data_ + result.size_, c, count);
        result.size_ += count;
    }
    result.data_[result.size_] = '\0';
    swap(result);
}
