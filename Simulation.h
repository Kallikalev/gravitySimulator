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
    constexpr const static float G = 100; // gravitational constant
    constexpr const static float density = 1.0f/5; // mass per unit area

    struct PhysicsObject {
        glm::vec2 position; // in world coordinates
        glm::vec2 velocity; // in units per second
        float mass;
        float radius; // in terms of world size

        PhysicsObject(glm::vec2 position, glm::vec2 velocity, float mass) :
            position(position), velocity(velocity), mass(mass) {
            radius = (float)sqrt(mass / (density * M_PI));
        }

        PhysicsObject(glm::vec2 position, float mass) :
                PhysicsObject(position, glm::vec2(0.0f), mass) {}
    };

    Simulation();

    std::vector<PhysicsObject>* step(float deltaTime);

private:
    std::vector<PhysicsObject> objects;

};


#endif //GRAVITYSIMULATOR_SIMULATION_H
