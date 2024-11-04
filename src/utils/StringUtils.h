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
    ss << "[";
    for (size_t i = 0; i < N; ++i) {
        ss << arr[i];
        if (i < N - 1)
            ss << ", ";
    }
    ss << "]";
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
