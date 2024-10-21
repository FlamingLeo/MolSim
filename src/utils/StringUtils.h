/**
 * @file StringUtils.h
 * @brief Utility functions for working with strings.
 * @date 2024-10-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

namespace StringUtils
{
    /**
     * @brief Converts a string to a double.
     *
     * @param str A reference to the string to be converted.
     * @return double
     */
    double convertStringToDouble(const std::string &str)
    {
        try
        {
            return std::stod(str, 0);
        }
        catch (const std::invalid_argument &ia)
        {
            std::cerr << "ERROR: Could not convert " << str << " to decimal!\n";
            exit(1);
        }
        catch (const std::out_of_range &oor)
        {
            std::cerr << "ERROR: Number " << str << " out of conversion range!\n";
            exit(1);
        }
    }
}
