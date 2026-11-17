#include "bank_product.h"

#include <cmath>
#include <istream>
#include <ostream>

BankProduct::operator double() const {
    return metric();
}

bool BankProduct::operator==(const BankProduct& other) const {
    return std::abs(metric() - other.metric()) < eps;
}

bool BankProduct::operator!=(const BankProduct& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const BankProduct& product) {
    product.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, BankProduct& product) {
    product.read(in);
    return in;
}
