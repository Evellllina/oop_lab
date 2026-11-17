#include "bank_product.h"
#include "card.h"
#include "credit.h"
#include "deposit.h"
#include "product_collection.h"

#include <gtest/gtest.h>

#include <sstream>
#include <stdexcept>
#include <string>

TEST(DepositTest, Metric) {
    EXPECT_NEAR(Deposit(1000.0, 0.1, 2.0, false).metric(), 100.0, 1e-6);
    EXPECT_NEAR(Deposit(1000.0, 0.1, 2.0, true).metric(), 105.0, 1e-6);
}

TEST(CreditTest, Metric) {
    EXPECT_NEAR(Credit(1000.0, 0.12, 1.0, false).metric(), 120.0, 1e-6);
    EXPECT_NEAR(Credit(120000.0, 12.0, 1.0, true).metric(), 127942.25569681206, 1e-6);
}

TEST(CardTest, Metric) {
    EXPECT_DOUBLE_EQ(Card(50000.0, 0.02).metric(), 1000.0);
}

TEST(PolymorphismTest, SpecificMethods) {
    ProductCollection products;
    products.add(Deposit(1000.0, 0.1, 2.0, true));
    products.add(Credit(1000.0, 12.0, 1.0, true));
    products.add(Card(1000.0, 0.05));
    auto* deposit = dynamic_cast<Deposit*>(&products.at(0));
    auto* credit = dynamic_cast<Credit*>(&products.at(1));
    auto* card = dynamic_cast<Card*>(&products.at(2));
    ASSERT_NE(deposit, nullptr);
    ASSERT_NE(credit, nullptr);
    ASSERT_NE(card, nullptr);
    EXPECT_TRUE(deposit->isCumulative());
    EXPECT_TRUE(credit->isAnnuity());
    EXPECT_TRUE(card->hasCashback());
}

TEST(PolymorphismTest, WrongCast) {
    Deposit deposit;
    BankProduct* product = &deposit;
    EXPECT_EQ(dynamic_cast<Card*>(product), nullptr);
}

TEST(OperatorTest, Equality) {
    const Deposit deposit(1000.0, 0.1, 1.0, false);
    EXPECT_TRUE(deposit == Card(2000.0, 0.05));
    EXPECT_TRUE(deposit == Card(1.0, 100.0 + BankProduct::eps / 2.0));
    EXPECT_TRUE(deposit != Card(1.0, 100.0 + BankProduct::eps * 2.0));
}

TEST(OperatorTest, DoubleConversion) {
    const Card card(1000.0, 0.05);
    const BankProduct& product = card;
    EXPECT_DOUBLE_EQ(static_cast<double>(product), 50.0);
}

TEST(InputTest, ReadFields) {
    Deposit deposit;
    Credit credit;
    Card card;
    std::istringstream first("1000 0.1 2 1");
    std::istringstream second("1000 0.12 1 0");
    std::istringstream third("50000 0.02");
    ASSERT_TRUE(first >> deposit);
    ASSERT_TRUE(second >> credit);
    ASSERT_TRUE(third >> card);
    EXPECT_NEAR(deposit.metric(), 105.0, 1e-6);
    EXPECT_NEAR(credit.metric(), 120.0, 1e-6);
    EXPECT_DOUBLE_EQ(card.metric(), 1000.0);
}

TEST(OutputTest, NamesAndFields) {
    std::ostringstream out;
    out << Deposit(1000.0, 0.5, 2.0, true) << '\n'
        << Credit(1000.0, 12.0, 1.0, true) << '\n'
        << Card(2000.0, 0.5);
    EXPECT_NE(out.str().find("Deposit: sum=1000 rate=0.5 years=2 cumulative=1 metric=625"), std::string::npos);
    EXPECT_NE(out.str().find("Credit: sum=1000 rate=12 years=1 annuity=1 metric="), std::string::npos);
    EXPECT_NE(out.str().find("Card: spending=2000 rate=0.5 metric=1000"), std::string::npos);
}

TEST(InputOutputTest, ReadPrintedObjects) {
    std::stringstream stream;
    stream << Deposit(1000.0, 0.5, 2.0, true) << '\n'
           << Credit(120000.0, 0.0, 2.0, true) << '\n'
           << Card(2000.0, 0.5);
    Deposit deposit;
    Credit credit;
    Card card;
    ASSERT_TRUE(stream >> deposit >> credit >> card);
    EXPECT_DOUBLE_EQ(deposit.metric(), 625.0);
    EXPECT_DOUBLE_EQ(credit.metric(), 60000.0);
    EXPECT_DOUBLE_EQ(card.metric(), 1000.0);
}

TEST(ValidationTest, InvalidParameters) {
    EXPECT_THROW(Deposit(-1.0, 0.1, 1.0, false), std::invalid_argument);
    EXPECT_THROW(Deposit(1000.0, 0.1, 0.0, true), std::invalid_argument);
    EXPECT_THROW(Credit(1000.0, 12.0, -1.0, true), std::invalid_argument);
    EXPECT_THROW(Card(1000.0, -0.1), std::invalid_argument);
}

TEST(CollectionTest, EmptyAndZeroValues) {
    ProductCollection products;
    EXPECT_TRUE(products.empty());
    EXPECT_EQ(products.size(), 0u);
    EXPECT_DOUBLE_EQ(products.totalMetric(), 0.0);
    EXPECT_NO_THROW(products.sortByMetric());
    EXPECT_DOUBLE_EQ(Deposit().metric(), 0.0);
    EXPECT_DOUBLE_EQ(Credit().metric(), 0.0);
    EXPECT_DOUBLE_EQ(Card().metric(), 0.0);
}

TEST(CollectionTest, TotalMetric) {
    ProductCollection products;
    products.add(Deposit(1000.0, 0.1, 2.0, true));
    products.add(Credit(1000.0, 0.2, 1.0, false));
    products.add(Card(1000.0, 0.05));
    EXPECT_EQ(products.size(), 3u);
    EXPECT_NEAR(products.totalMetric(), 355.0, 1e-6);
}

TEST(CollectionTest, SortByMetric) {
    ProductCollection products;
    products.add(Credit(1000.0, 0.3, 1.0, false));
    products.add(Deposit(1000.0, 0.2, 1.0, false));
    products.add(Card(1000.0, 0.1));
    products.sortByMetric();
    EXPECT_DOUBLE_EQ(products.at(0).metric(), 100.0);
    EXPECT_DOUBLE_EQ(products.at(1).metric(), 200.0);
    EXPECT_DOUBLE_EQ(products.at(2).metric(), 300.0);
}

TEST(OwnershipTest, DeleteThroughBasePointer) {
    BankProduct* deposit = new Deposit(1000.0, 0.1, 1.0, false);
    BankProduct* credit = new Credit(1000.0, 0.2, 1.0, false);
    BankProduct* card = new Card(1000.0, 0.05);
    ProductCollection products;
    products.add(*deposit);
    products.add(*credit);
    products.add(*card);
    delete deposit;
    delete credit;
    delete card;
    products.remove(0);
    EXPECT_EQ(products.size(), 2u);
    products.clear();
    EXPECT_TRUE(products.empty());
}
