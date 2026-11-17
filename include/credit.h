#pragma once

#include "bank_product.h"

class Credit final : public BankProduct {
public:
    Credit() = default;
    Credit(double sum, double rate, double years, bool annuity);
    double metric() const override;
    void print(std::ostream& out) const override;
    void read(std::istream& in) override;
    Credit* clone() const override;
    bool isAnnuity() const noexcept;

private:
    double sum_ = 0.0;
    double rate_ = 0.0;
    double years_ = 1.0;
    bool annuity_ = false;
};
