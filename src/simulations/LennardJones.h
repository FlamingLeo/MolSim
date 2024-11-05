//
// Created by marag on 11/5/2024.
//

#ifndef PSEMOLDYN_GROUPC_LENNARDJONES_H
#define PSEMOLDYN_GROUPC_LENNARDJONES_H

#endif //PSEMOLDYN_GROUPC_LENNARDJONES_H

#pragma once
#include "Simulation.h"
#include "objects/ParticleContainer.h"
#include "utils/Arguments.h"
#include <string>

class LennardJones : public Simulation {
    private:


    public:
        LennardJones(const std::string &filename, const Arguments &args);

        ~LennardJones();

        void runSimulation() override;
};
