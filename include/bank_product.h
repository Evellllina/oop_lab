#pragma once

#include <iosfwd>

class BankProduct {
public:
    static constexpr double eps = 1e-6;
    BankProduct() = default;
    BankProduct(const BankProduct&) = default;
    BankProduct(BankProduct&&) = default;
    BankProduct& operator=(const BankProduct&) = default;
    BankProduct& operator=(BankProduct&&) = default;
    virtual ~BankProduct() = default;

    virtual double metric() const = 0;
    virtual void print(std::ostream& out) const = 0;
    virtual void read(std::istream& in) = 0;
    virtual BankProduct* clone() const = 0;
    explicit operator double() const;
    bool operator==(const BankProduct& other) const;
    bool operator!=(const BankProduct& other) const;
};

std::ostream& operator<<(std::ostream& out, const BankProduct& product);
std::istream& operator>>(std::istream& in, BankProduct& product);
