#include "WriterFactory.h"

namespace outputWriter {
std::unique_ptr<FileWriter> createWriter(outputWriter::WriterType type) {
    switch (type) {
    case outputWriter::WriterType::XYZ:
        return std::make_unique<XYZWriter>();
    case outputWriter::WriterType::VTK:
        return std::make_unique<VTKWriter>();
    default:
        CLIUtils::error("Invalid file writer type!");
    }
};
} // namespace outputWriter