//
// Created by Kalev Martinson on 8/15/24.
//

#include "Simulation.h"

namespace {

}

Simulation::Simulation() : topSubdivision(Subdivision(-WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, -WORLD_SIZE, nullptr))  {
    unsigned int subdivisionCount = 5; // how many times should we subdivide the grid
    Subdivision::subdivide(&topSubdivision, subdivisionCount);
    Subdivision::buildAdjacency(&topSubdivision);


    for (int x = -100; x <= 100; x += 10) {
        for (int y = -100; y <= 100; y += 10) {
            objects.emplace_back(glm::vec2(x, y), 5);
        }
    }
}

std::vector<PhysicsObject>* Simulation::step(float deltaTime) {
    topSubdivision.clearObjects();

    for (PhysicsObject& obj : objects) {
        topSubdivision.placeObject(&obj);
    }

    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            float dist = glm::length(objects[i].position - objects[j].position);
            if (dist <= std::max(objects[i].radius, objects[j].radius)) {
                // velocity via conservation of momentum
                objects[i].velocity = (objects[i].velocity * objects[i].mass +
                                       objects[j].velocity * objects[j].mass)
                                      / (objects[i].mass + objects[j].mass);
                // center of mass via weighted average
                objects[i].position = (objects[i].position * objects[i].mass +
                                       objects[j].position * objects[j].mass)
                                      / (objects[i].mass + objects[j].mass);
                objects[i].mass += objects[j].mass;
                objects.erase(objects.begin() + j);
                j--;
                objects[i].correctRadius();
            }
        }
    }

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

    return &objects;
}
