//
// Created by marag on 11/5/2024.
//
#include "array"
#include "ParticleContainer.h"

#ifndef PSEMOLDYN_GROUPC_CUBOID_H
#define PSEMOLDYN_GROUPC_CUBOID_H

#endif //PSEMOLDYN_GROUPC_CUBOID_H

class Cuboid{
    private:

        std::array<double, 3> position;

        std::array<int, 3> size;

        double h;

        double m;

        std::array<double, 3> v;

        double mean_velocity;

        ParticleContainer particles;

    public:
        Cuboid(const std::array<double, 3> &position, const std::array<int, 3> &size, const std::array<double, 3> &v, double h, double m);

        ParticleContainer initializeParticles();
};