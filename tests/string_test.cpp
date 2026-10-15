#include "string.h"
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <utility>


TEST(StringBasic, DefaultConstructorIsEmpty) {
    String s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_GE(s.capacity(), 1u);
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringBasic, ConstructFromCString) {
    String s("Hello");
    EXPECT_EQ(s.size(), 5u);
    EXPECT_STREQ(s.c_str(), "Hello");
    EXPECT_EQ(s[0], 'H');
    EXPECT_EQ(s.at(4), 'o');
    EXPECT_THROW(s.at(5), std::out_of_range);
}

TEST(StringBasic, ConstructFromNullptrIsEmpty) {
    String s(nullptr);
    EXPECT_TRUE(s.empty());
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringBasic, MoveAssignment) {
    String a("x");
    String b("moved");
    a = std::move(b);
    EXPECT_STREQ(a.c_str(), "moved");
    EXPECT_TRUE(b.empty());
}

TEST(StringBasic, PushBackDoublesCapacity) {
    String s;
    EXPECT_EQ(s.capacity(), 1u);
    s.push_back('a');
    EXPECT_EQ(s.capacity(), 1u);
    s.push_back('b');
    EXPECT_EQ(s.capacity(), 2u);
    s.push_back('c');
    EXPECT_EQ(s.capacity(), 4u);
    s.push_back('d');
    s.push_back('e');
    EXPECT_EQ(s.capacity(), 8u);
    EXPECT_STREQ(s.c_str(), "abcde");
}

TEST(StringBasic, AppendSelf) {
    String s("ab");
    s.append(s);
    EXPECT_STREQ(s.c_str(), "abab");
    s.append(s.c_str());
    EXPECT_STREQ(s.c_str(), "abababab");
}

TEST(StringBasic, Find) {
    String s("Hello, World!");
    EXPECT_EQ(s.find("World"), 7u);
    EXPECT_EQ(s.find("Hello"), 0u);
    EXPECT_EQ(s.find("xyz"), String::npos);
    EXPECT_EQ(s.find(""), 0u);
    EXPECT_EQ(String().find("a"), String::npos);
    EXPECT_EQ(String("aaa").find("a", 1), 1u);
}

TEST(StringBasic, PrintAndClear) {
    String s("print me");
    std::ostringstream os;
    s.print(os);
    EXPECT_EQ(os.str(), "print me");
    s.clear();
    EXPECT_TRUE(s.empty());
    EXPECT_STREQ(s.c_str(), "");
}

TEST(StringBasic, SwapExchangesContents) {
    String a("one");
    String b("two!");
    a.swap(b);
    EXPECT_STREQ(a.c_str(), "two!");
    EXPECT_STREQ(b.c_str(), "one");
}

TEST(StringRle, CompressExampleFromTask) {
    String s("AAABCC");
    EXPECT_STREQ(s.rle_compress().c_str(), "A3B1C2");
    EXPECT_STREQ(s.c_str(), "AAABCC"); 
}

TEST(StringRle, DecompressExampleFromTask) {
    String s("AAABCC");
    String packed = s.rle_compress();
    packed.rle_decompress();
    EXPECT_STREQ(packed.c_str(), "AAABCC");
}

TEST(StringRle, EmptyString) {
    String s;
    String packed = s.rle_compress();
    EXPECT_TRUE(packed.empty());
    packed.rle_decompress();
    EXPECT_TRUE(packed.empty());
}

TEST(StringRle, SingleCharacter) {
    String s("x");
    EXPECT_STREQ(s.rle_compress().c_str(), "x1");
    String p("x1");
    p.rle_decompress();
    EXPECT_STREQ(p.c_str(), "x");
}

TEST(StringRle, RoundTripWithSpacesAndPunctuation) {
    String s("  !!..aaa  b");
    String packed = s.rle_compress();
    EXPECT_STREQ(packed.c_str(), " 2!2.2a3 2b1");
    packed.rle_decompress();
    EXPECT_EQ(packed, s);
}

