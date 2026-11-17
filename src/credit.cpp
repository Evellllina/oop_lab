#include "credit.h"
#include "io_utils.h"

#include <cmath>
#include <ostream>

Credit::Credit(double sum, double rate, double years, bool annuity)
    : sum_(sum), rate_(rate), years_(years), annuity_(annuity) {
    bank_detail::requireNonnegative(sum_, "sum");
    bank_detail::requireNonnegative(rate_, "rate");
    bank_detail::requireYears(years_);
    bank_detail::requireFiniteMetric(metric());
}

double Credit::metric() const {
    if (sum_ == 0.0) {
        return 0.0;
    }
    if (!annuity_) {
        return sum_ * rate_;
    }
    if (rate_ == 0.0) {
        return sum_ / years_;
    }
    const long double monthly = static_cast<long double>(rate_) / 1200.0L;
    const long double months = static_cast<long double>(years_) * 12.0L;
    const long double denominator = -std::expm1(-months * std::log1p(monthly));
    return static_cast<double>(12.0L * sum_ * monthly / denominator);
}

void Credit::print(std::ostream& out) const {
    auto text = bank_detail::output();
    text << "Credit: sum=" << sum_ << " rate=" << rate_ << " years=" << years_
         << " annuity=" << static_cast<int>(annuity_) << " metric=" << metric();
    out << text.str();
}

void Credit::read(std::istream& in) {
    double sum = 0.0;
    double rate = 0.0;
    double years = 0.0;
    int annuity = 0;
    const bool named = bank_detail::labeled(in, 'C');
    if (named) {
        if (!bank_detail::consume(in, "Credit:") ||
            !bank_detail::readValue(in, "sum=", sum) ||
            !bank_detail::readValue(in, "rate=", rate) ||
            !bank_detail::readValue(in, "years=", years) ||
            !bank_detail::readValue(in, "annuity=", annuity)) {
            return;
        }
    } else if (!(in >> sum >> rate >> years >> annuity)) {
        return;
    }
    try {
        Credit next(sum, rate, years, annuity != 0);
        if (!named || bank_detail::readMetric(in, next.metric())) {
            *this = next;
        }
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
    }
}

Credit* Credit::clone() const {
    return new Credit(*this);
}

bool Credit::isAnnuity() const noexcept {
    return annuity_;
}
