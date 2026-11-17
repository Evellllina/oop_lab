#pragma once

#include "bank_product.h"

class Deposit final : public BankProduct {
public:
    Deposit() = default;
    Deposit(double sum, double rate, double years, bool cumulative);
    double metric() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    Deposit* clone() const override;
    bool isCumulative() const noexcept;

private:
    double sum_ = 0.0;
    double rate_ = 0.0;
    double years_ = 1.0;
    bool cumulative_ = false;
};
