#include "deposit.h"
#include "io_utils.h"

#include <cmath>
#include <ostream>

Deposit::Deposit(double sum, double rate, double years, bool cumulative)
    : sum_(sum), rate_(rate), years_(years), cumulative_(cumulative) {
    bank_detail::requireNonnegative(sum_, "sum");
    bank_detail::requireNonnegative(rate_, "rate");
    bank_detail::requireYears(years_);
    bank_detail::requireFiniteMetric(metric());
}

double Deposit::metric() const {
    if (sum_ == 0.0 || rate_ == 0.0) {
        return 0.0;
    }
    if (!cumulative_) {
        return sum_ * rate_;
    }
    const long double growth = std::expm1(
        static_cast<long double>(years_) * std::log1p(static_cast<long double>(rate_)));
    return static_cast<double>(static_cast<long double>(sum_) * growth / years_);
}

void Deposit::print(std::ostream& out) const {
    auto text = bank_detail::output();
    text << "Deposit: sum=" << sum_ << " rate=" << rate_ << " years=" << years_
         << " cumulative=" << static_cast<int>(cumulative_) << " metric=" << metric();
    out << text.str();
}

void Deposit::read(std::istream& in) {
    double sum = 0.0;
    double rate = 0.0;
    double years = 0.0;
    int cumulative = 0;
    const bool named = bank_detail::labeled(in, 'D');
    if (named) {
        if (!bank_detail::consume(in, "Deposit:") ||
            !bank_detail::readValue(in, "sum=", sum) ||
            !bank_detail::readValue(in, "rate=", rate) ||
            !bank_detail::readValue(in, "years=", years) ||
            !bank_detail::readValue(in, "cumulative=", cumulative)) {
            return;
        }
    } else if (!(in >> sum >> rate >> years >> cumulative)) {
        return;
    }
    try {
        Deposit next(sum, rate, years, cumulative != 0);
        if (!named || bank_detail::readMetric(in, next.metric())) {
            *this = next;
        }
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
    }
}

Deposit* Deposit::clone() const {
    return new Deposit(*this);
}

bool Deposit::isCumulative() const noexcept {
    return cumulative_;
}
