//
// Created by Kalev Martinson on 8/15/24.
//

#ifndef GRAVITYSIMULATOR_SIMULATION_H
#define GRAVITYSIMULATOR_SIMULATION_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Simulation {
public:
    unsigned int WORLD_SIZE = 100; // number of arbitrary length units from the center to the edge of the world
    float G = 100; // gravitational constant

    struct PhysicsObject {
        glm::vec2 position; // in world coordinates
        glm::vec2 velocity; // in units per second
        float mass;
        float radius; // in terms of world size

        PhysicsObject(glm::vec2 position, glm::vec2 velocity, float mass, float radius) :
            position(position), velocity(velocity), mass(mass), radius(radius) {}

        PhysicsObject(glm::vec2 position, float mass, float radius) :
                position(position), velocity(glm::vec2(0.0f)), mass(mass), radius(radius) {}
    };

    Simulation();

    std::vector<PhysicsObject>* step(float deltaTime);

private:
    std::vector<PhysicsObject> objects;

};


#endif //GRAVITYSIMULATOR_SIMULATION_H
