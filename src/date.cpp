#include "date.h"

Date::Date() : year_(1970), month_(1), day_(1) {}
Date::Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

bool Date::is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int Date::days_in_month(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return is_leap(year) ? 29 : 28;
        default:
            return 0;
    }
}

bool Date::is_valid() const {
    if (year_ < 1 || year_ > 9999) return false;
    if (month_ < 1 || month_ > 12) return false;
    return day_ >= 1 && day_ <= days_in_month(year_, month_);
}

bool Date::operator==(const Date& other) const {
    return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
}

namespace {

void print_padded(std::ostream& os, int value, int width) {
    if (value < 0) {
        os << '-';
        value = -value;
    }
    int digits = 1;
    for (int v = value; v >= 10; v /= 10) ++digits;
    for (int i = digits; i < width; ++i) os << '0';
    os << value;
}

}

std::ostream& operator<<(std::ostream& os, const Date& d) {
    print_padded(os, d.year_, 4);
    os << '-';
    print_padded(os, d.month_, 2);
    os << '-';
    print_padded(os, d.day_, 2);
    return os;
}
