#pragma once

#include <iostream>

class Date {
public:
    Date();
    Date(int year, int month, int day);

    int year() const { return year_; }
    int month() const { return month_; }
    int day() const { return day_; }

    bool is_valid() const;

    static bool is_leap(int year);
    static int days_in_month(int year, int month);

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Date& d);

private:
    int year_;
    int month_;
    int day_;
};
