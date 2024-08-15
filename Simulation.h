//
// Created by Kalev Martinson on 8/15/24.
//

#ifndef GRAVITYSIMULATOR_SIMULATION_H
#define GRAVITYSIMULATOR_SIMULATION_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Subdivision.h"

class Simulation {
public:
    constexpr const static float WORLD_SIZE = 100; // number of arbitrary length units from the center to the edge of the world
    constexpr const static float G = 100; // gravitational constant

    Subdivision topSubdivision;

    Simulation();

    std::vector<PhysicsObject>* step(float deltaTime);

private:
    std::vector<PhysicsObject> objects;

};


#endif //GRAVITYSIMULATOR_SIMULATION_H
