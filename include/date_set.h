#pragma once

#include <cstddef>
#include <iostream>

#include "date.h"

class DateSet {
public:
    DateSet();
    DateSet(const DateSet& other);
    DateSet(DateSet&& other) noexcept;
    DateSet& operator=(const DateSet& other);
    DateSet& operator=(DateSet&& other) noexcept;
    ~DateSet();

    void swap(DateSet& other) noexcept;

    bool add(const Date& d);
    bool remove(const Date& d);
    bool contains(const Date& d) const;
    bool operator()(const Date& d) const;

    std::size_t size() const;
    bool empty() const;
    std::size_t capacity() const;
    const Date& operator[](std::size_t i) const;

    DateSet& operator+=(const Date& d);
    DateSet& operator-=(const Date& d);

    DateSet operator|(const DateSet& other) const;
    DateSet operator&(const DateSet& other) const;
    DateSet operator-(const DateSet& other) const;

    bool operator==(const DateSet& other) const;
    bool operator!=(const DateSet& other) const;

    friend std::ostream& operator<<(std::ostream& os, const DateSet& s);

private:
    std::size_t lower_bound(const Date& d) const;
    void reserve(std::size_t new_capacity);
    void push_back_sorted(const Date& d);

    Date* data_;
    std::size_t size_;
    std::size_t capacity_;
};
