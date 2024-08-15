//
// Created by Kalev Martinson on 8/15/24.
//

#ifndef GRAVITYSIMULATOR_PHYSICSOBJECT_H
#define GRAVITYSIMULATOR_PHYSICSOBJECT_H

#include <glm/glm.hpp>


struct PhysicsObject {
    constexpr const static float density = 2.0f; // mass per unit area

    glm::vec2 position; // in world coordinate
    glm::vec2 velocity; // in units per second
    float mass;
    float radius{}; // in terms of world size

    void correctRadius() {
        radius = (float)sqrt(mass / (density * M_PI));
    }

    PhysicsObject(glm::vec2 position, glm::vec2 velocity, float mass) :
            position(position), velocity(velocity), mass(mass) {
        correctRadius();
    }

    PhysicsObject(glm::vec2 position, float mass) :
            PhysicsObject(position, glm::vec2(0.0f), mass) {}
};


#endif //GRAVITYSIMULATOR_PHYSICSOBJECT_H
