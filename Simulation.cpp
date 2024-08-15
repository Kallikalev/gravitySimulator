//
// Created by Kalev Martinson on 8/15/24.
//

#include "Simulation.h"

Simulation::Simulation()  {
    objects.emplace_back(glm::vec2(20.0f, 0.0f), 5.0f);
    objects.emplace_back(glm::vec2(-50.0f, 25.0f), 25.0f);
}

float speed = 10.0f; // distance units per second

std::vector<Simulation::PhysicsObject>* Simulation::step(float deltaTime) {
    glm::vec2 mov0 = (objects[1].position - objects[0].position);
    mov0 = mov0/glm::length(mov0);
    mov0 = mov0 * speed;
    objects[0].position = objects[0].position + mov0 * deltaTime;

    glm::vec2 mov1 = (objects[0].position - objects[1].position);
    mov1 = mov1/glm::length(mov1);
    mov1 = mov1 * speed;
    objects[1].position = objects[1].position + mov1 * deltaTime;
    return &objects;
}