#include <gtest/gtest.h>

#include <sstream>
#include <utility>

#include "date.h"
#include "date_set.h"

namespace {

std::string to_string(const DateSet& s) {
    std::ostringstream os;
    os << s;
    return os.str();
}

Date next_day(const Date& d) {
    int y = d.year(), m = d.month(), day = d.day() + 1;
    if (day > Date::days_in_month(y, m)) {
        day = 1;
        if (++m > 12) {
            m = 1;
            ++y;
        }
    }
    return Date(y, m, day);
}

DateSet make(std::initializer_list<Date> dates) {
    DateSet s;
    for (const Date& d : dates) s.add(d);
    return s;
}

}

TEST(DateSet, EmptySet) {
    DateSet s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.capacity(), 0u);
    EXPECT_FALSE(s.contains(Date(2024, 1, 1)));
    EXPECT_FALSE(s.remove(Date(2024, 1, 1)));
}

TEST(DateSet, AddNewAndDuplicate) {
    DateSet s;
    EXPECT_TRUE(s.add(Date(2024, 3, 15)));
    EXPECT_FALSE(s.add(Date(2024, 3, 15)));
    EXPECT_EQ(s.size(), 1u);
}

TEST(DateSet, RemoveExistingAndMissing) {
    DateSet s = make({Date(2024, 1, 1), Date(2024, 2, 1), Date(2024, 3, 1)});
    EXPECT_TRUE(s.remove(Date(2024, 2, 1)));
    EXPECT_FALSE(s.remove(Date(2024, 2, 1)));
    EXPECT_FALSE(s.remove(Date(2025, 1, 1)));
    EXPECT_EQ(to_string(s), "{ 2024-01-01, 2024-03-01 }");
}

TEST(DateSet, ContainsMatchesCallOperator) {
    DateSet s = make({Date(2024, 1, 1), Date(2024, 5, 9)});
    const Date probes[] = {Date(2024, 1, 1), Date(2024, 5, 9), Date(2024, 1, 2), Date(1999, 1, 1)};
    for (const Date& d : probes) {
        EXPECT_EQ(s.contains(d), s(d)) << d;
    }
    EXPECT_TRUE(s(Date(2024, 5, 9)));
    EXPECT_FALSE(s(Date(2024, 1, 2)));
}

TEST(DateSet, CopyIsIndependent) {
    DateSet a = make({Date(2024, 1, 1), Date(2024, 2, 1)});
    DateSet b(a);
    b.add(Date(2024, 3, 1));
    b.remove(Date(2024, 1, 1));
    EXPECT_EQ(to_string(a), "{ 2024-01-01, 2024-02-01 }");

    DateSet c;
    c = a;
    c.add(Date(2020, 1, 1));
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(c.size(), 3u);
}

TEST(DateSet, MoveLeavesSourceEmpty) {
    DateSet a = make({Date(2024, 1, 1), Date(2024, 2, 1)});
    DateSet b(std::move(a));
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.capacity(), 0u);
    EXPECT_EQ(to_string(a), "{ }");

    EXPECT_TRUE(a.add(Date(2030, 1, 1)));
    EXPECT_EQ(a.size(), 1u);

    DateSet c;
    c = std::move(b);
    EXPECT_EQ(c.size(), 2u);
    EXPECT_TRUE(b.empty());
}

TEST(DateSet, UnionIntersectionDifference) {
    DateSet a = make({Date(2024, 1, 1), Date(2024, 2, 1), Date(2024, 3, 1)});
    DateSet b = make({Date(2024, 2, 1), Date(2024, 3, 1), Date(2024, 4, 1)});

    EXPECT_EQ(to_string(a | b), "{ 2024-01-01, 2024-02-01, 2024-03-01, 2024-04-01 }");
    EXPECT_EQ(to_string(a & b), "{ 2024-02-01, 2024-03-01 }");
    EXPECT_EQ(to_string(a - b), "{ 2024-01-01 }");
    EXPECT_EQ(to_string(b - a), "{ 2024-04-01 }");
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 3u);
    DateSet empty;
    EXPECT_EQ(a | empty, a);
    EXPECT_TRUE((a & empty).empty());
    EXPECT_EQ(a - empty, a);
    EXPECT_TRUE((empty - a).empty());
}

TEST(DateSet, EqualityConsistent) {
    DateSet a = make({Date(2024, 1, 1), Date(2024, 2, 1), Date(2024, 3, 1)});
    DateSet b = make({Date(2024, 3, 1), Date(2024, 1, 1), Date(2024, 2, 1)});
    DateSet c = make({Date(2024, 1, 1), Date(2024, 2, 1)});
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_EQ(DateSet(), DateSet());
}

TEST(DateSet, PrintFormat) {
    EXPECT_EQ(to_string(DateSet()), "{ }");
    EXPECT_EQ(to_string(make({Date(2024, 3, 5)})), "{ 2024-03-05 }");
    EXPECT_EQ(to_string(make({Date(2024, 3, 5), Date(2023, 1, 1)})),
              "{ 2023-01-01, 2024-03-05 }");
}

TEST(DateSet, Stress1000) {
    Date dates[1000];
    Date d(2000, 1, 1);
    for (int i = 0; i < 1000; ++i) {
        dates[i] = d;
        d = next_day(d);
    }

    DateSet s;
    for (int i = 999; i >= 0; --i) {
        EXPECT_TRUE(s.add(dates[i]));
    }
    for (int i = 0; i < 1000; ++i) {
        EXPECT_FALSE(s.add(dates[i]));
    }
    EXPECT_EQ(s.size(), 1000u);
    for (std::size_t i = 1; i < s.size(); ++i) {
        EXPECT_TRUE(s[i - 1] < s[i]);
    }
    for (int i = 0; i < 1000; ++i) {
        EXPECT_TRUE(s.contains(dates[i]));
    }
}

TEST(DateSet, RejectsInvalidDates) {
    DateSet s;
    EXPECT_FALSE(s.add(Date(2023, 2, 29)));
    EXPECT_FALSE(s.add(Date(2024, 13, 1)));
    EXPECT_FALSE(s.add(Date(2024, 4, 31)));
    s += Date(2024, 0, 10);
    EXPECT_TRUE(s.empty());
    EXPECT_TRUE(s.add(Date(2024, 2, 29)));
    EXPECT_FALSE(s.contains(Date(2023, 2, 29)));
}

TEST(DateSet, KeepsSortedOrder) {
    DateSet s;
    s += Date(2024, 5, 1);
    s += Date(2020, 1, 1);
    s += Date(2024, 1, 15);
    s += Date(2030, 12, 31);
    s += Date(2024, 1, 14);
    EXPECT_EQ(to_string(s), "{ 2020-01-01, 2024-01-14, 2024-01-15, 2024-05-01, 2030-12-31 }");
}

TEST(DateSet, RemoveFirstLastOnly) {
    DateSet s = make({Date(2024, 1, 1), Date(2024, 2, 1), Date(2024, 3, 1)});
    EXPECT_TRUE(s.remove(Date(2024, 3, 1)));
    EXPECT_TRUE(s.remove(Date(2024, 1, 1)));
    EXPECT_EQ(to_string(s), "{ 2024-02-01 }");
    EXPECT_TRUE(s.remove(Date(2024, 2, 1)));
    EXPECT_TRUE(s.empty());
    EXPECT_TRUE(s.add(Date(2024, 2, 1)));
}

TEST(DateSet, CompoundOperatorsChain) {
    DateSet s;
    (s += Date(2024, 1, 1)) += Date(2024, 2, 1);
    s += Date(2024, 3, 1);
    (s -= Date(2024, 1, 1)) -= Date(2099, 1, 1);
    EXPECT_EQ(to_string(s), "{ 2024-02-01, 2024-03-01 }");
}

TEST(DateSet, SelfAssignmentAndSelfMove) {
    DateSet s = make({Date(2024, 1, 1), Date(2024, 2, 1)});
    DateSet& ref = s;
    s = ref;
    EXPECT_EQ(to_string(s), "{ 2024-01-01, 2024-02-01 }");
    s = std::move(ref);
    EXPECT_EQ(to_string(s), "{ 2024-01-01, 2024-02-01 }");
}

TEST(DateSet, CopyAndMoveFromEmpty) {
    DateSet empty;
    DateSet copy(empty);
    EXPECT_TRUE(copy.empty());
    DateSet moved(std::move(empty));
    EXPECT_TRUE(moved.empty());
    EXPECT_TRUE(empty.empty());
    DateSet copy2(empty);
    EXPECT_TRUE(copy2.add(Date(2024, 1, 1)));
}

TEST(DateSet, CapacityDoubles) {
    DateSet s;
    EXPECT_EQ(s.capacity(), 0u);
    s += Date(2024, 1, 1);
    EXPECT_EQ(s.capacity(), 1u);
    s += Date(2024, 1, 2);
    EXPECT_EQ(s.capacity(), 2u);
    s += Date(2024, 1, 3);
    EXPECT_EQ(s.capacity(), 4u);
    s += Date(2024, 1, 4);
    s += Date(2024, 1, 5);
    EXPECT_EQ(s.capacity(), 8u);
}

TEST(DateSet, AddOwnElement) {
    DateSet s = make({Date(2024, 1, 1)});
    DateSet t = make({Date(2024, 1, 2)});
    EXPECT_FALSE(s.add(s[0]));
    EXPECT_TRUE(t.add(s[0]));
    EXPECT_EQ(to_string(t), "{ 2024-01-01, 2024-01-02 }");
}
