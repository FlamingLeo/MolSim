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

    for (const auto &s : invalids) {
        EXPECT_DEATH(StringUtils::toInt(s), "") << "Tested with string " << s;
    }
}

// Test converting strings containing valid integers to integers.
TEST(StringUtilsTests, ToIntArrayValid) {
    const std::string s1 = "{1,2,3}";
    constexpr std::array arr1 = {1, 2, 3};
    EXPECT_EQ(StringUtils::toIntArray<3>(s1), arr1);
}