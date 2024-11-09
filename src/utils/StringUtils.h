/**
 * @file StringUtils.h
 * @brief Utility functions for working with strings.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "Arguments.h"
#include "CLIUtils.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>

/// @brief Map containing conversion information for converting a string to a WriterType enum.
static inline const std::unordered_map<std::string, WriterType> writerTable = {
    {"vtk", WriterType::VTK}, {"xyz", WriterType::XYZ}, {"nil", WriterType::NIL}};

/// @brief Map containing conversion information for converting a string to a SimulationType enum.
static inline const std::unordered_map<std::string, SimulationType> simulationTable = {
    {"verlet", SimulationType::VERLET}};

/// @brief Reverse map containing conversion information for converting a WriterType enum to a string.
static inline const std::unordered_map<WriterType, std::string> writerStringTable = []() {
    std::unordered_map<WriterType, std::string> reverseMap;
    for (const auto &pair : writerTable)
        reverseMap[pair.second] = pair.first;
    return reverseMap;
}();

/**
 * @brief Finds a given substring in a string. Case-insensitive.
 *
 * Source: https://stackoverflow.com/a/19839371
 *
 * @param haystack The string which may or may not contain the substring.
 * @param needle The substring to search for in the string.
 * @return true if the string contains the substring, case-insensitive.
 * @return false if the string does not contain the substring, case-insensitive.
 */
static inline bool findStringIC(const std::string &haystack, const std::string &needle) {
    auto it = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end(),
                          [](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
    return (it != haystack.end());
}

/// @brief Reverse map containing conversion information for converting a SimulationType enum to a string.
static inline const std::unordered_map<SimulationType, std::string> simulationStringTable = []() {
    std::unordered_map<SimulationType, std::string> reverseMap;
    for (const auto &pair : simulationTable)
        reverseMap[pair.second] = pair.first;
    return reverseMap;
}();

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
        // invalidate NAN, INF and hex numbers
        if (findStringIC(str, "inf") || findStringIC(str, "nan") || findStringIC(str, "0x"))
            throw std::invalid_argument("Invalid number");

        // convert singular passed number as string
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
 * @brief Converts a string into an integer array.
 *
 * The string must be of the format "{_, _, ..., _}" (excluding quotes) and contain exactly as many elements as
 * specified. If the string is empty, however, an empty array will always be returned. If the provided length of array
 * is longer than the amount of elements read, the remaining array slots will be filled with zeroes. If the provided
 * length of the array is shorter than the amount of elements read, the array will be truncated.
 *
 * Each number may have an arbitrary amount of leading whitespaces. Trailing whitespaces are not allowed.
 *
 * @tparam N The length of the array.
 * @param str The string to be converted, should be formatted as comma-separated integers between curly braces.
 * @return The corresponding array of integers.
 */
template <size_t N> static inline std::array<int, N> toIntArray(const std::string &str) {
    if (str.empty() || N == 0)
        return {};

    if (str.front() != '{' || str.back() != '}')
        CLIUtils::error("Invalid array syntax", str);

    std::array<int, N> arr = {};
    std::string tmp;
    std::string numbers = str.substr(1, str.size() - 2);
    std::stringstream ss(numbers);

    size_t i = 0;
    while (i < N && std::getline(ss, tmp, ',')) {
        arr[i++] = StringUtils::toInt(tmp);
    }

    return arr;
}

/**
 * @brief Converts a string into a double array.
 *
 * The string must be of the format "{_, _, ..., _}" (excluding quotes) and contain exactly as many elements as
 * specified. If the string is empty, however, an empty array will always be returned. If the provided length of array
 * is longer than the amount of elements read, the remaining array slots will be filled with zeroes. If the provided
 * length of the array is shorter than the amount of elements read, the array will be truncated.
 *
 * Each number may have an arbitrary amount of leading whitespaces. Trailing whitespaces are not allowed.
 *
 * @tparam N The length of the array.
 * @param str The string to be converted, should be formatted as comma-separated decimal numbers between curly braces.
 * @return The corresponding array of doubles.
 */
template <size_t N> static inline std::array<double, N> toDoubleArray(const std::string &str) {
    if (str.empty() || N == 0)
        return {};

    if (str.front() != '{' || str.back() != '}')
        CLIUtils::error("Invalid array syntax", str);

    std::array<double, N> arr = {};
    std::string tmp;
    std::string numbers = str.substr(1, str.size() - 2);
    std::stringstream ss(numbers);

    size_t i = 0;
    while (i < N && std::getline(ss, tmp, ',')) {
        arr[i++] = StringUtils::toDouble(tmp);
    }

    return arr;
}

/**
 * @brief Converts a string to a WriterType enum using a dedicated map.
 *
 * @param type The string containing the desired WriterType.
 * @return The desired WriterType enum if the type string is valid, otherwise terminates with error.
 */
static inline WriterType toWriterType(const std::string &type) {
    auto it = writerTable.find(type);
    if (it != writerTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return WriterType::VTK; // shouldn't reach this; included to silence warning
}

/**
 * @brief Converts a string to a SimulationType enum using a dedicated map.
 *
 * @param type The string containing the desired SimulationType.
 * @return The desired SimulationType enum if the type string is valid, otherwise terminates with error.
 */
static inline SimulationType toSimulationType(const std::string &type) {
    auto it = simulationTable.find(type);
    if (it != simulationTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return SimulationType::VERLET; // shouldn't reach this; included to silence warning
}

/**
 * @brief Converts a char to a string.
 *
 * @param c The character to be converted.
 * @return The resulting string.
 */
static inline std::string fromChar(char c) {
    std::string s{c};
    return s;
}

/**
 * @brief Converts a WriterType to a string.
 *
 * @param writerType An instance of the WriterType enum to be converted.
 * @return The resulting string.
 */
static inline std::string fromWriterType(WriterType writerType) { return writerStringTable.at(writerType); }

/**
 * @brief Converts a SimulationType to a string.
 *
 * @param writerType An instance of the SimulationType enum to be converted.
 * @return The resulting string.
 */
static inline std::string fromSimulationType(SimulationType simulationType) {
    return simulationStringTable.at(simulationType);
}

/**
 * @brief Converts a generic array to a string.
 *
 * @tparam T The type of the array elements.
 * @tparam N The length of the array.
 * @param arr The array to be converted.
 * @return A string representation of the array elements enclosed in square brackets.
 */
template <typename T, size_t N> static inline std::string fromArray(const std::array<T, N> &arr) {
    std::stringstream ss;
    ss << "{";
    for (size_t i = 0; i < N; ++i) {
        ss << arr[i];
        if (i < N - 1)
            ss << ", ";
    }
    ss << "}";
    return ss.str();
}

/**
 * @brief Converts a number to a string.
 *
 * @tparam T The type of the number.
 * @param number The number to be converted.
 * @return A string representation of the number.
 */
template <typename T> std::string static inline fromNumber(T number) { return std::to_string(number); }

} // namespace StringUtils
