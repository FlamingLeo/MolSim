#include "WriterFactory.h"
#include <memory>
#include <string>

std::unique_ptr<FileWriter> WriterFactory::createWriter(WriterType type, const std::string &basename) {
    switch (type) {
    case WriterType::XYZ:
        SPDLOG_DEBUG("Generating XYZWriter...");
        return std::make_unique<XYZWriter>(basename);
    case WriterType::VTK:
        SPDLOG_DEBUG("Generating VTKWriter...");
        return std::make_unique<VTKWriter>(basename);
    case WriterType::NIL:
        SPDLOG_DEBUG("Generating NullWriter...");
        return std::make_unique<NullWriter>(basename);
    default:
        CLIUtils::error("Invalid file writer type!", "", false);
    }
    return std::make_unique<XYZWriter>();
}
