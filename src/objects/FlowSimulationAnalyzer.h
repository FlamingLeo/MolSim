#include "ParticleContainer.h"
#include <vector>

class FlowSimulationAnalyzer {

    private:

    ParticleContainer &particles;

    int binNumber;

    double leftWallXPos;

    double rightWallXPos;

    double binSize;

    std::vector<double> densities;

    std::vector<double> velocities;

    public:

    FlowSimulationAnalyzer(ParticleContainer &particles, int binNumber, double leftWallXPos, double rightWallXPos);

    void calculateDensitiesAndVelocities();

    int getBinNumber() const;

    double getLeftWallXPos() const;

    double getRightWallXPos() const;

    double getBinSize() const;

    ParticleContainer &getParticles() const;

    const std::vector<double>& getDensities() const;

    const std::vector<double>& getVelocities() const;
};