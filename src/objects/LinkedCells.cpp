#include "LinkedCells.h"
#include "utils/Arguments.h"
#include <cmath>


LinkedCells::LinkedCells() {

    ParticleContainer  m_particles = ParticleContainer();
    m_particles.fromFile("../../input/input-test.txt");

    //Here additional parameters should be read through FileReader from XML, I'll give them default values
    cutoff = 3;
    domainSize = {180, 90, 0};


    //determining cell size
    for(int i = 0; i < 3; i++){

        //Special case to avoid issues when actually working with 2D
        if(i == 2 && domainSize[i] == 0){
            std::cout << "we in here\n";
            cellSize[2] = 0;
            numCells[2] = 3;
            continue;
        }

        if(std::fabs(std::fmod(domainSize[i], cutoff)) < 1e-9){
            //perfect fit
            cellSize[i] = cutoff;
            numCells[i] = std::round(domainSize[i] / cellSize[i]) + 2;
        } else{
            //we round up cellsize so that cell > cutoff and we only search adjacent cells
            cellSize[i] = domainSize[i] / std::floor(domainSize[i] / cutoff) ;
            numCells[i] = std::round(domainSize[i] / cellSize[i]) + 2;
        }
    }
    //reserve space for all cells
    std::cout << "numcells z is " << numCells[2];
    std::cout<< "num of cells is : " << numCells[0] * numCells[1] * numCells[2] << "\n";
    cells.reserve(numCells[0] * numCells[1] * numCells[2]);

    //Creating cells
    for(int z = 0; z < numCells[2]; z++){
        std::cout<< "z is " << z << "\n";
        for(int y = 0; y < numCells[1]; y++){
            for(int x = 0; x < numCells[0]; x++){

                   bool halo = (z == 0 || z == (numCells[2] - 1) ||
                                y == 0 || y == (numCells[1] - 1) ||
                                x == 0 || x == (numCells[0] - 1));

                   bool border = (z == 1 || z == (numCells[2] - 2) ||
                                    y == 1 || y == (numCells[1] - 2) ||
                                    x == 1 || x == (numCells[0] - 2));


                   CellType type;
                   if(halo){
                       type = CellType::HALO;
                   } else if(border){
                       type = CellType::BORDER;
                   } else{
                       type = CellType::INNER;
                   }

                   //position of lower left corner
                   std::array<double, 3> position =  {x * cellSize[0], y * cellSize[1], z * cellSize[2]};

                   Cell c = Cell(cellSize, position, type);

                   cells.push_back(c);

                   if(type == CellType::HALO){
                       halo_cells.push_back(&c);
                   } else if(type == CellType::BORDER){
                       border_cells.push_back(&c);
                   }

            }
        }
    }

    //adding particles
    for(Particle p : m_particles){
        addParticle(p);
    }
}

int LinkedCells::getCellIndex(const std::array<double, 3>& position) {
    std::array<int, 3> coords;
    for (int i = 0; i < 3; ++i) {

        //if you divide 0 all hell breaks loose
        if(position[i] == 0){
            coords[i] = 0;
            continue;
        }

        coords[i] = static_cast<int>(std::floor(position[i] / cellSize[i])); // +1 for halo offset
        if (coords[i] < 0 || coords[i] >= numCells[i]) {
            return -1; // Outside the bounds
        }
    }
    return coords[2] * numCells[0] * numCells[1] + coords[1] * numCells[0] + coords[0];
}

// Add a particle to the appropriate cell
void LinkedCells::addParticle(const Particle &particle) {
    int cellIndex = getCellIndex(particle.getX());
    std::cout << "index is " << cellIndex << "\n";
    if (cellIndex >= 0 && cellIndex < static_cast<int>(cells.size())) {
        cells[cellIndex].addParticle(const_cast<Particle &>(particle));
    } else {
        std::cout << "bad addParticle\n";
    }
}

void LinkedCells::deleteParticle(Particle &p) {
    //TODO
}

// Get the (x, y, z) coordinates of a cell from its 1D index
std::array<int, 3> LinkedCells::getVirtualCellCoordinates(int index) {
    int x = index % numCells[0];
    int y = (index / numCells[0]) % numCells[1];
    int z = index / (numCells[0] * numCells[1]);
    return {x, y, z};
}

std::vector<Cell> LinkedCells::getCells() {
    return cells;
}

//return indexes of ALL neighbours
std::vector<int> LinkedCells::getNeighbors3D(int cellIndex) {
    std::vector<int> neighbors;
    std::array<int, 3> coords = getVirtualCellCoordinates(cellIndex);

    for (int dz = -1; dz <= 1; ++dz) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {

                if (dx == 0 && dy == 0 && dz == 0) {
                    continue; // Skip the current cell
                }

                std::array<int, 3> neighborCoords = {
                        coords[0] + dx, coords[1] + dy, coords[2] + dz};

                if (neighborCoords[0] >= 0 && neighborCoords[0] < numCells[0] &&
                    neighborCoords[1] >= 0 && neighborCoords[1] < numCells[1] &&
                    neighborCoords[2] >= 0 && neighborCoords[2] < numCells[2]) {
                    int neighborIndex = neighborCoords[2] * numCells[1] * numCells[0] + neighborCoords[1] * numCells[0] + neighborCoords[0];
                    neighbors.push_back(neighborIndex);
                }
            }
        }
    }

    return neighbors;
}

void LinkedCells::calculateForce() {
    double epsilon = 5;
    double sigma = 1;
    for(Cell &c1 : cells){
        for(Cell &c2 : cells){
            calculateF_LennardJones2(c1.getParticles(), c2.getParticles(), epsilon, sigma);
        }
    }
}

void LinkedCells::calculateVelocity() {
    double delta_t = 0.0005;
    for(Cell &c : cells){
        calculateV(c.getParticles(), delta_t);
    }
}

void LinkedCells::calculatePosition(){
    double delta_t = 0.0005;
    std::cout<< "in calculatePosition\n";
    //update position within particle object
    for(Cell &c : cells){
        calculateX(c.getParticles(), delta_t);
        for(Particle &p : c.getParticles()){
            std::cout<< p.toString() << "\n";
        }
    }


    //move particles to proper cell
    //again, this could be improved, kinda bruteforced, maybe store vector of particles that are gonna change cell
    //and move them all at the end so you don't iterate over them twice

    for(std::size_t i = 0; i < cells.size(); i++){
        for(Particle &p: cells[i].getParticles()){
            int actual = getCellIndex(p.getX());
            if(actual >= (int) cells.size() || actual < 0){
                std::cout << "bad index " << actual << " on particle " << p.toString() << "\n";
                //we delete particle outside the perimeter
                cells[i].removeParticle(p);
                continue;
            }

            if(actual != (int) i){
                cells[actual].addParticle(p);
                cells[i].removeParticle(p);
            }
        }
    }

}

