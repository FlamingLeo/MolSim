/*
 * VTKWriter.cpp
 *
 *  Created on: 01.03.2010
 *      Author: eckhardw
 */

#include "VTKWriter.h"
#include "utils/CLIUtils.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <filesystem>

namespace outputWriter
{
  VTKWriter::VTKWriter() = default;
  VTKWriter::VTKWriter(const std::string& basename) : m_basename {basename} {};
  VTKWriter::VTKWriter(const std::string& basename, const std::string& dirname) : m_basename {basename}, m_dirname {dirname} {};

  VTKWriter::~VTKWriter() = default;

  void VTKWriter::initializeOutput(int numParticles)
  {
    // initialize new vtk file
    m_vtkFile = new VTKFile_t("UnstructuredGrid");
    if (!m_vtkFile)
      CLIUtils::error("Error initializing VTK file!");

    // per point, we add type, position, velocity and force
    PointData pointData;
    DataArray_t mass(type::Float32, "mass", 1);
    DataArray_t velocity(type::Float32, "velocity", 3);
    DataArray_t forces(type::Float32, "force", 3);
    DataArray_t type(type::Int32, "type", 1);
    pointData.DataArray().push_back(mass);
    pointData.DataArray().push_back(velocity);
    pointData.DataArray().push_back(forces);
    pointData.DataArray().push_back(type);

    CellData cellData; // we don't have cell data => leave it empty

    // 3 coordinates
    Points points;
    DataArray_t pointCoordinates(type::Float32, "points", 3);
    points.DataArray().push_back(pointCoordinates);

    Cells cells; // we don't have cells, => leave it empty
    // for some reasons, we have to add a dummy entry for paraview
    DataArray_t cells_data(type::Float32, "types", 0);
    cells.DataArray().push_back(cells_data);

    PieceUnstructuredGrid_t piece(pointData, cellData, points, cells,
                                  numParticles, 0);
    UnstructuredGrid_t unstructuredGrid(piece);
    m_vtkFile->UnstructuredGrid(unstructuredGrid);
  }

  void VTKWriter::writeFile(int iteration)
  {
    // before we do anything, check if there is something valid to write
    if (!m_vtkFile)
      CLIUtils::error("Cannot write uninitialized VTK file!");

    // create output directory in which to store generated VTK output files
    if (!(std::filesystem::exists(m_dirname)))
    {
      if (!(std::filesystem::create_directory(m_dirname)))
        CLIUtils::error("Error creating VTK directory!");
    }

    // generate unique filename based on iteration
    std::stringstream strstr;
    strstr << m_dirname << "/" << m_basename << "_" << std::setfill('0') << std::setw(4) << iteration << ".vtu";

    std::ofstream file(strstr.str().c_str());

    if (!file)
      CLIUtils::error("Error opening output file", m_basename);

    // write file using vtk library
    VTKFile(file, *m_vtkFile);
    delete m_vtkFile;
  }

  void VTKWriter::plotParticle(const Particle &p)
  {
    // check for valid vtk file
    if (!m_vtkFile || !(m_vtkFile->UnstructuredGrid().present()))
      CLIUtils::error("VTK file incorrectly initialized!");

    // do some dark vtk magic
    PointData::DataArray_sequence &pointDataSequence =
        m_vtkFile->UnstructuredGrid()->Piece().PointData().DataArray();
    PointData::DataArray_iterator dataIterator = pointDataSequence.begin();

    dataIterator->push_back(p.getM());

    dataIterator++;
    dataIterator->push_back(p.getV()[0]);
    dataIterator->push_back(p.getV()[1]);
    dataIterator->push_back(p.getV()[2]);

    dataIterator++;
    dataIterator->push_back(p.getOldF()[0]);
    dataIterator->push_back(p.getOldF()[1]);
    dataIterator->push_back(p.getOldF()[2]);

    dataIterator++;
    dataIterator->push_back(p.getType());

    Points::DataArray_sequence &pointsSequence =
        m_vtkFile->UnstructuredGrid()->Piece().Points().DataArray();
    Points::DataArray_iterator pointsIterator = pointsSequence.begin();
    pointsIterator->push_back(p.getX()[0]);
    pointsIterator->push_back(p.getX()[1]);
    pointsIterator->push_back(p.getX()[2]);
  }

  void VTKWriter::writeParticles(const std::list<Particle> &particles, int iteration)
  {
    initializeOutput(particles.size());

    for (auto &p : particles)
      plotParticle(p);

    writeFile(iteration);
  }

} // namespace outputWriter
