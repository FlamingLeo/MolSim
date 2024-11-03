#include "WriterFactory.h"

std::unique_ptr<FileWriter> createWriter(WriterType type) {
    switch (type) {
    case WriterType::XYZ:
        SPDLOG_DEBUG("Generating XYZWriter...");
        return std::make_unique<XYZWriter>();
    case WriterType::VTK:
        SPDLOG_DEBUG("Generating VTKWriter...");
        return std::make_unique<VTKWriter>();
    default:
        CLIUtils::error("Invalid file writer type!");
    }
    return std::make_unique<XYZWriter>();
};
