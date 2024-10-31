/**
 * @file StringUtils.h
 * @brief Utility functions for working with strings.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "utils/CLIUtils.h"
#include <iostream>
#include <stdexcept>
#include <string>

/// @brief  Namespace defining utility functions for working with strings.
namespace StringUtils {
/**
 * @brief Converts a string to a double.
 *
 * @param str A reference to the string to be converted.
 * @return The converted string as a double.
 */
static inline double toDouble(const std::string &str) {
    try {
        size_t idx = 0;
        double converted = std::stod(str, &idx);
        if (idx != str.length())
            throw std::invalid_argument("Invalid number");
        return converted;
    } catch (const std::invalid_argument &ia) {
        CLIUtils::error("Could not convert to decimal", str);
    } catch (const std::out_of_range &oor) {
        CLIUtils::error("Number out of conversion range", str);
    }
    // shouldn't reach this; only here to "mute" compiler warning
    return 0.0;
}

/**
 * @brief Converts a string to an integer.
 *
 * @param str A reference to the string to be converted.
 * @return The converted string as an int.
 */
static inline double toInt(const std::string &str) {
    try {
        size_t idx = 0;
        int converted = std::stoi(str, &idx);
        if (idx != str.length())
            throw std::invalid_argument("Invalid number");
        return converted;
    } catch (const std::invalid_argument &ia) {
        CLIUtils::error("Could not convert to integer", str);
    } catch (const std::out_of_range &oor) {
        CLIUtils::error("Number out of conversion range", str);
    }
    // shouldn't reach this; only here to "mute" compiler warning
    return 0;
}

/**
 * @brief Converts a char to a string.
 *
 * @param c The character to be converted.
 * @return The resulting string.
 */
static inline std::string fromString(char c) {
    std::string s{c};
    return s;
}
} // namespace StringUtils
