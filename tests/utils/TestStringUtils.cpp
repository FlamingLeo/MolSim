#include "utils/StringUtils.h"
#include <gtest/gtest.h>
#include <string>
#include <unordered_map>

// Test converting strings containing valid doubles to doubles.
TEST(StringUtilsTests, ToDoubleValid) {
    const std::unordered_map<std::string, double> table{{"0", 0.0},       {"-.6", -.6},   {"+3.14", +3.14},
                                                        {"-2.71", -2.71}, {"10e6", 10e6}, {"-13e37", -13e37}};

    for (const auto &[key, val] : table) {
        EXPECT_DOUBLE_EQ(StringUtils::toDouble(key), val) << "Tested with string " << key;
    }
}

// Test attempting to convert strings containing invalid doubles.
TEST(StringUtilsTests, ToDoubleInvalid) {
    constexpr std::array invalids{"INF",
                                  "INFINITY",
                                  "Inf",
                                  "INFinity",
                                  "inf",
                                  "infinity",
                                  "NAN",
                                  "nan",
                                  "NAn",
                                  "NAN123",
                                  "nan123",
                                  "NAn123",
                                  "0xBEEF",
                                  "0XF00D"
                                  "0b10101",
                                  "0B110011",
                                  "HUGE_VAL",
                                  "bogus",
                                  "2e999",
                                  "-2e999"};

    GTEST_FLAG_SET(death_test_style, "threadsafe");
    for (const auto &s : invalids) {
        EXPECT_DEATH(StringUtils::toDouble(s), "") << "Tested with string " << s;
    }
}

// Test converting strings containing valid integers to integers.
TEST(StringUtilsTests, ToIntValid) {
    const std::unordered_map<std::string, int> table{
        {"0", 0}, {"+42", 42}, {"-13", -13}, {"2147483647", 2147483647}, {"-2147483648", -2147483648}};

    for (const auto &[key, val] : table) {
        EXPECT_EQ(StringUtils::toInt(key), val) << "Tested with string " << key;
    }
}

// Test attempting to convert strings containing invalid integers.
TEST(StringUtilsTests, ToIntInvalid) {
    constexpr std::array invalids{"INF",
                                  "INFINITY",
                                  "Inf",
                                  "INFinity",
                                  "inf",
                                  "infinity",
                                  "NAN",
                                  "nan",
                                  "NAn",
                                  "NAN123",
                                  "nan123",
                                  "NAn123",
                                  "0xBEEF",
                                  "0XF00D"
                                  "0b10101",
                                  "0B110011",
                                  "HUGE_VAL",
                                  "bogus",
                                  "1e10",
                                  "-2e20",
                                  "1.234",
                                  "-5.678",
                                  ".1337",
                                  "-.4242",
                                  "2147483648",
                                  "-2147483649"};

    GTEST_FLAG_SET(death_test_style, "threadsafe");
    for (const auto &s : invalids) {
        EXPECT_DEATH(StringUtils::toInt(s), "") << "Tested with string " << s;
    }
}

// Test converting strings containing valid integer arrays to integer arrays.
TEST(StringUtilsTests, ToIntArrayValid) {
    const std::string s0 = "{}";
    constexpr std::array<int, 0> arr0 = {};
    EXPECT_EQ(StringUtils::toIntArray<0>(s0), arr0);

    const std::string s1 = "{1, 2, 3}";
    constexpr std::array arr1 = {1, 2, 3};
    EXPECT_EQ(StringUtils::toIntArray<3>(s1), arr1);

    const std::string s2 = "{        4,   5,                       6}";
    constexpr std::array arr2 = {4, 5, 6};
    EXPECT_EQ(StringUtils::toIntArray<3>(s2), arr2);

    const std::string s3 = "{7, 8, 9}";
    constexpr std::array arr3 = {7, 8, 9, 0, 0, 0};
    EXPECT_EQ(StringUtils::toIntArray<6>(s3), arr3);

    const std::string s4 = "{10, 11, 12}";
    constexpr std::array arr4 = {10};
    EXPECT_EQ(StringUtils::toIntArray<1>(s4), arr4);
}

// Test different ways of returning an empty integer array from a string.
TEST(StringUtilsTests, ToIntArrayEmpty) {
    constexpr std::array<int, 0> empty = {};
    constexpr std::array<int, 10> empty10 = {};

    const std::string s0 = "{}";
    EXPECT_EQ(StringUtils::toIntArray<0>(s0), empty);

    const std::string s1 = "";
    EXPECT_EQ(StringUtils::toIntArray<0>(s1), empty);
    EXPECT_EQ(StringUtils::toIntArray<10>(s1), empty10);

    const std::string s2 = "bogus";
    EXPECT_EQ(StringUtils::toIntArray<0>(s2), empty);
}

// Test attempting to convert strings containing invalid integer arrays.
TEST(StringUtilsTests, ToIntArrayInvalid) {
    GTEST_FLAG_SET(death_test_style, "threadsafe");

    const std::string s0 = "invalid";
    EXPECT_DEATH(StringUtils::toIntArray<5>(s0), "");

    const std::string s1 = "{1 ,2 ,3 }";
    EXPECT_DEATH(StringUtils::toIntArray<3>(s1), "");
}

// Test converting strings containing valid integer arrays to double arrays.
TEST(StringUtilsTests, ToDoubleArrayValid) {
    const std::string s0 = "{}";
    constexpr std::array<double, 0> arr0 = {};
    EXPECT_EQ(StringUtils::toDoubleArray<0>(s0), arr0);

    const std::string s1 = "{1.0, 2.0, 3.0}";
    constexpr std::array arr1 = {1., 2., 3.};
    EXPECT_EQ(StringUtils::toDoubleArray<3>(s1), arr1);

    const std::string s2 = "{        4.,   5,                       6.0}";
    constexpr std::array arr2 = {4., 5., 6.};
    EXPECT_EQ(StringUtils::toDoubleArray<3>(s2), arr2);

    const std::string s3 = "{7.0, 8.0, 9.0}";
    constexpr std::array arr3 = {7., 8., 9., 0., 0., 0.};
    EXPECT_EQ(StringUtils::toDoubleArray<6>(s3), arr3);

    const std::string s4 = "{10.0, 11.0, 12.0}";
    constexpr std::array arr4 = {10.};
    EXPECT_EQ(StringUtils::toDoubleArray<1>(s4), arr4);
}

// Test different ways of returning an empty double array from a string.
TEST(StringUtilsTests, ToDoubleArrayEmpty) {
    constexpr std::array<double, 0> empty = {};
    constexpr std::array<double, 10> empty10 = {};

    const std::string s0 = "{}";
    EXPECT_EQ(StringUtils::toDoubleArray<0>(s0), empty);

    const std::string s1 = "";
    EXPECT_EQ(StringUtils::toDoubleArray<0>(s1), empty);
    EXPECT_EQ(StringUtils::toDoubleArray<10>(s1), empty10);

    const std::string s2 = "bogus";
    EXPECT_EQ(StringUtils::toDoubleArray<0>(s2), empty);
}

// Test attempting to convert strings containing invalid double arrays.
TEST(StringUtilsTests, ToDoubleArrayInvalid) {
    GTEST_FLAG_SET(death_test_style, "threadsafe");

    const std::string s0 = "invalid";
    EXPECT_DEATH(StringUtils::toDoubleArray<5>(s0), "");

    const std::string s1 = "{1. ,2. ,3. }";
    EXPECT_DEATH(StringUtils::toDoubleArray<3>(s1), "");
}