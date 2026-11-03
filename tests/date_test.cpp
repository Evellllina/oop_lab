#include <gtest/gtest.h>
#include <sstream>
#include "date.h"

TEST(Date, DefaultIsValid) {
    Date d;
    EXPECT_TRUE(d.is_valid());
    EXPECT_EQ(d, Date(1970, 1, 1));
}

TEST(Date, LeapYears) {
    EXPECT_TRUE(Date::is_leap(2024));
    EXPECT_FALSE(Date::is_leap(2023));
    EXPECT_FALSE(Date::is_leap(1900));
    EXPECT_TRUE(Date::is_leap(2000));
}

TEST(Date, Validation) {
    EXPECT_TRUE(Date(2024, 2, 29).is_valid());
    EXPECT_FALSE(Date(2023, 2, 29).is_valid());
    EXPECT_FALSE(Date(1900, 2, 29).is_valid());
    EXPECT_TRUE(Date(2024, 1, 31).is_valid());
    EXPECT_FALSE(Date(2024, 4, 31).is_valid());
    EXPECT_FALSE(Date(2024, 13, 1).is_valid());
    EXPECT_FALSE(Date(2024, 0, 1).is_valid());
    EXPECT_FALSE(Date(2024, 1, 0).is_valid());
    EXPECT_FALSE(Date(0, 1, 1).is_valid());
    EXPECT_FALSE(Date(2024, -1, 10).is_valid());
}

TEST(Date, EqualityAndOrder) {
    EXPECT_EQ(Date(2024, 3, 15), Date(2024, 3, 15));
    EXPECT_NE(Date(2024, 3, 15), Date(2024, 3, 16));
    EXPECT_TRUE(Date(2023, 12, 31) < Date(2024, 1, 1));
    EXPECT_TRUE(Date(2024, 1, 31) < Date(2024, 2, 1));
    EXPECT_TRUE(Date(2024, 2, 1) < Date(2024, 2, 2));
    EXPECT_FALSE(Date(2024, 2, 2) < Date(2024, 2, 2));
}

TEST(Date, PrintWithLeadingZeros) {
    std::ostringstream os;
    os << Date(2024, 3, 5) << ' ' << Date(987, 12, 31);
    EXPECT_EQ(os.str(), "2024-03-05 0987-12-31");
}
