#include "card.h"
#include "io_utils.h"

#include <ostream>

Card::Card(double spending, double rate) : spending_(spending), rate_(rate) {
    bank_detail::requireNonnegative(spending_, "spending");
    bank_detail::requireNonnegative(rate_, "rate");
    bank_detail::requireFiniteMetric(metric());
}

double Card::metric() const {
    return spending_ * rate_;
}

void Card::print(std::ostream& out) const {
    auto text = bank_detail::output();
    text << "Card: spending=" << spending_ << " rate=" << rate_ << " metric=" << metric();
    out << text.str();
}

void Card::read(std::istream& in) {
    double spending = 0.0;
    double rate = 0.0;
    const bool named = bank_detail::labeled(in, 'C');
    if (named) {
        if (!bank_detail::consume(in, "Card:") ||
            !bank_detail::readValue(in, "spending=", spending) ||
            !bank_detail::readValue(in, "rate=", rate)) {
            return;
        }
    } else if (!(in >> spending >> rate)) {
        return;
    }
    try {
        Card next(spending, rate);
        if (!named || bank_detail::readMetric(in, next.metric())) {
            *this = next;
        }
    } catch (const std::invalid_argument&) {
        in.setstate(std::ios::failbit);
    }
}

Card* Card::clone() const {
    return new Card(*this);
}

bool Card::hasCashback() const noexcept {
    return rate_ > 0.0;
}
