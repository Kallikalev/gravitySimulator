//
// Created by Kalev Martinson on 8/15/24.
//

#include "Simulation.h"

Simulation::Simulation()  {
    for (int x = -100; x <= 100; x += 10) {
        for (int y = -100; y <= 100; y += 10) {
                objects.emplace_back(glm::vec2(x, y), 5);
        }
    }
}

std::vector<Simulation::PhysicsObject>* Simulation::step(float deltaTime) {
    for (int i = 0; i < objects.size(); i++) {
        for (int j = 0; j < objects.size(); j++) {
            if (i != j) {
                glm::vec2 diff = objects[j].position - objects[i].position;
                float len = glm::length(diff);
                glm::vec2 dir = diff / len;
                float force = G * objects[i].mass * objects[j].mass / (len * len);
                float acc = force / objects[i].mass;
                objects[i].velocity += acc * dir * deltaTime;
            }
        }
    }

    for (PhysicsObject& object : objects) {
        object.position += object.velocity * deltaTime;
    }

//    objects[0].position = glm::vec2(0.1f);

    return &objects;
}