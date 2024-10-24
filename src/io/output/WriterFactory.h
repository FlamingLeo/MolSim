/**
 * @file WriterFactory.h
 * @brief Factory class for creating different types of file output generators.
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "FileWriter.h"
#include "VTKWriter.h"
#include "XYZWriter.h"
#include "utils/Arguments.h"
#include "utils/CLIUtils.h"
#include <memory>

namespace outputWriter {
/**
 * @brief Returns a unique pointer to a default-initialized writer object of a given type.
 *
 * @param type The specified outputWriter::WriterType of the writer object to be instantiated.
 * @return A std::unique_ptr<FileWriter> instance matching the desired type.
 */
std::unique_ptr<FileWriter> createWriter(outputWriter::WriterType type);
} // namespace outputWriter