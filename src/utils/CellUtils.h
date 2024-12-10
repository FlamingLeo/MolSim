/**
 * @file CellUtils.h
 * @brief Utility functions for Cell and CellContainer objects.
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include "CLIUtils.h"
#include <bitset>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/// @brief Enum containing the possible types of boundary conditions.
enum class BoundaryCondition { OUTFLOW, REFLECTIVE, PERIODIC };
/// @brief Enum containing the possible types of cells.
enum class CellType { INNER, BORDER, HALO };
/// @brief Enum containing the possible corner and edge directions of halo cells.
enum class HaloLocation { NORTH, SOUTH, WEST, EAST, ABOVE, BELOW };
/// @brief Enum containing the possible corner and edge directions of border cells
enum class BorderLocation {NORTH, SOUTH, WEST, EAST, ABOVE, BELOW};

/// @brief Namespace containing utility functions for Cell and CellContainer objects.
namespace CellUtils {
/// @brief Map containing conversion information for converting a string to a BoundaryCondition enum.
static inline const std::unordered_map<std::string, BoundaryCondition> conditionTable = {
    {"outflow", BoundaryCondition::OUTFLOW},
    {"reflective", BoundaryCondition::REFLECTIVE},
    {"periodic", BoundaryCondition::PERIODIC}};

/// @brief Map containing conversion information for converting a string to a CellType enum.
static inline const std::unordered_map<std::string, CellType> typeTable = {
    {"inner", CellType::INNER}, {"border", CellType::BORDER}, {"halo", CellType::HALO}};

/// @brief Map containing conversion information for converting a string to a HaloLocation enum.
static inline const std::unordered_map<std::string, HaloLocation> haloTable = {
    {"north", HaloLocation::NORTH}, {"south", HaloLocation::SOUTH}, {"east", HaloLocation::EAST},
    {"west", HaloLocation::WEST},   {"above", HaloLocation::ABOVE}, {"below", HaloLocation::BELOW},
};

/// @brief Reverse map containing conversion information for converting a BoundaryCondition enum to a string.
static inline const std::unordered_map<BoundaryCondition, std::string> conditionStringTable = []() {
    std::unordered_map<BoundaryCondition, std::string> reverseMap;
    for (const auto &pair : conditionTable)
        reverseMap[pair.second] = pair.first;
    return reverseMap;
}();

/// @brief Reverse map containing conversion information for converting a CellType enum to a string.
static inline const std::unordered_map<CellType, std::string> typeStringTable = []() {
    std::unordered_map<CellType, std::string> reverseMap;
    for (const auto &pair : typeTable)
        reverseMap[pair.second] = pair.first;
    return reverseMap;
}();

/// @brief Reverse map containing conversion information for converting a HaloLocation enum to a string.
static inline const std::unordered_map<HaloLocation, std::string> haloStringTable = []() {
    std::unordered_map<HaloLocation, std::string> reverseMap;
    for (const auto &pair : haloTable)
        reverseMap[pair.second] = pair.first;
    return reverseMap;
}();

/**
 * @brief Converts a BoundaryCondition to a string.
 *
 * @param type An instance of the BoundaryCondition enum to be converted.
 * @return The resulting string.
 */
static inline std::string fromBoundaryCondition(BoundaryCondition type) { return conditionStringTable.at(type); }

/**
 * @brief Converts a string to a BoundaryCondition enum using a dedicated map.
 *
 * @param type The string containing the desired BoundaryCondition.
 * @return The desired BoundaryCondition enum if the type string is valid, otherwise terminates with error.
 */
static inline BoundaryCondition toBoundaryCondition(const std::string &type) {
    auto it = conditionTable.find(type);
    if (it != conditionTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return BoundaryCondition::OUTFLOW; // shouldn't reach this; included to silence warning
}

/**
 * @brief Converts a CellType to a string.
 *
 * @param type An instance of the CellType enum to be converted.
 * @return The resulting string.
 */
static inline std::string fromType(CellType type) { return typeStringTable.at(type); }

/**
 * @brief Converts a string to a CellType enum using a dedicated map.
 *
 * @param type The string containing the desired CellType.
 * @return The desired CellType enum if the type string is valid, otherwise terminates with error.
 */
static inline CellType toType(const std::string &type) {
    auto it = typeTable.find(type);
    if (it != typeTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return CellType::INNER; // shouldn't reach this; included to silence warning
}

/**
 * @brief Converts a HaloLocation to a string.
 *
 * @param type An instance of the HaloLocation enum to be converted.
 * @return The resulting string.
 */
static inline std::string fromHalo(HaloLocation type) { return haloStringTable.at(type); }

/**
 * @brief Converts a vector of HaloLocation enums to a truncated, concatenated string.
 *
 * For each type, the first letter is chosen and appended to the final string.
 *
 * @param vec The vector of HaloLocation enums.
 * @return A string containing the initials of all elements, in order.
 */
static inline std::string fromHaloVec(const std::vector<HaloLocation> &vec) {
    std::stringstream ss;
    for (auto v : vec) {
        switch (v) {
        case HaloLocation::NORTH:
            ss << "N";
            break;
        case HaloLocation::SOUTH:
            ss << "S";
            break;
        case HaloLocation::EAST:
            ss << "E";
            break;
        case HaloLocation::WEST:
            ss << "W";
            break;
        case HaloLocation::ABOVE:
            ss << "A";
            break;
        case HaloLocation::BELOW:
            ss << "B";
            break;
        default:
            SPDLOG_WARN("This shouldn't happen! (haloVec)");
            break;
        }
    }
    return ss.str();
}

/**
 * @brief Converts an array of BoundaryCondition enums to a truncated, concatenated string.
 *
 * For each type, the first letter is chosen and appended to the final string.
 *
 * @param arr The array of BoundaryCondition enums.
 * @return A string containing the initials of all elements, in order.
 */
static inline std::string fromBoundaryConditionArray(const std::array<BoundaryCondition, 6> &arr) {
    std::stringstream ss;
    for (auto c : arr) {
        switch (c) {
        case BoundaryCondition::OUTFLOW:
            ss << "o";
            break;
        case BoundaryCondition::REFLECTIVE:
            ss << "r";
            break;
        case BoundaryCondition::PERIODIC:
            ss << "p";
            break;
        default:
            SPDLOG_WARN("This shouldn't happen! (fromBoundaryConditionArray)");
            break;
        }
    }
    return ss.str();
}

/**
 * @brief Converts a string to a HaloLocation enum using a dedicated map.
 *
 * @param type The string containing the desired HaloLocation.
 * @return The desired HaloLocation enum if the type string is valid, otherwise terminates with error.
 */
static inline HaloLocation toHalo(const std::string &type) {
    auto it = haloTable.find(type);
    if (it != haloTable.end())
        return it->second;
    else
        CLIUtils::error("Invalid output type", type);
    return HaloLocation::NORTH; // shouldn't reach this; included to silence warning
}

/**
 * @brief Converts a string of length 6 to an array of boundary conditions for each boundary.
 *
 * The characters correspond to the boundary condition for the following directions (in this order): north, south, east,
 * west, above, below. The string may only contain the characters described in the help string.
 *
 * @param str The string to parse.
 * @return An array of boundary conditions for each boundary.
 */
static inline std::array<BoundaryCondition, 6> stringToBoundaryConditions(const std::string &str) {
    if (str.size() != 6)
        CLIUtils::error("Invalid boundary condition string length!");
    std::array<BoundaryCondition, 6> arr;
    size_t index = 0;
    for (auto &c : str) {
        switch (c) {
        case 'o':
            arr[index++] = BoundaryCondition::OUTFLOW;
            break;
        case 'r':
            arr[index++] = BoundaryCondition::REFLECTIVE;
            break;
        case 'p':
            arr[index++] = BoundaryCondition::PERIODIC;
            break;
        default:
            CLIUtils::error("Invalid boundary condition character (must be one of: o, r)!");
        }
    }
    return arr;
}

} // namespace CellUtils
