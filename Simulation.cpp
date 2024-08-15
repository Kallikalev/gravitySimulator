//
// Created by Kalev Martinson on 8/15/24.
//

#include "Simulation.h"

#include <queue>

namespace {
    struct Subdivision {
        float left;
        float right;
        float up;
        float down;
        Subdivision* parent;
        Subdivision* children[4]; // in order: top left, top right, bottom left, bottom right
        Subdivision* adjacent[8]; // in order, top left then going clockwise

        Subdivision(float left, float right, float up, float down, Subdivision* parent) :
            left(left), right(right), up(up), down(down), parent(parent), children(), adjacent() {}

        ~Subdivision() {
            for (auto & i : children) {
                delete i;
            }
        }
    };

    void subdivide(Subdivision* s, unsigned int depthRemaining) {
        if (depthRemaining < 1) {
            return;
        }
        // in order: top left, top right, bottom left, bottom right
        s->children[0] = new Subdivision(s->left, (s->left + s->right) / 2, s->up, (s->up + s->down) / 2, s);
        s->children[1] = new Subdivision((s->left + s->right) / 2, s->right , s->up, (s->up + s->down) / 2, s);
        s->children[2] = new Subdivision(s->left, (s->left + s->right) / 2, (s->up + s->down) / 2, s->down, s);
        s->children[3] = new Subdivision((s->left + s->right) / 2, s->right , (s->up + s->down) / 2, s->down, s);
        for (auto & i : s->children) {
            subdivide(i,depthRemaining-1);
        }
    }

    void buildAdjacency(Subdivision* s) {
        std::queue<Subdivision*> ptrQueue;
        std::queue<int> posQueue;
        ptrQueue.push(s);
        posQueue.push(0);
        while (!ptrQueue.empty()) {
            Subdivision* cur = ptrQueue.front();
            int curPos = posQueue.front();
            ptrQueue.pop();
            posQueue.pop();
            for (int i = 0; i < 4; i++) {
                if (cur->children[i] != nullptr) {
                    ptrQueue.push(cur->children[i]);
                    posQueue.push(i);
                }
            }
            if (cur->parent != nullptr) { // parent level subdivision has no adjacent
                if (curPos < 2) { // we are on the top side of the subdivision
                    if (cur->parent->adjacent[1] != nullptr) {
                        cur->adjacent[1] = cur->parent->adjacent[1]->children[curPos + 2];
                    }
                    cur->adjacent[5] = cur->parent->children[curPos+2];
                } else { // we are on the bottom side of the subdivision
                    cur->adjacent[1] = cur->parent->children[curPos-2];
                    if (cur->parent->adjacent[5] != nullptr) {
                        cur->adjacent[5] = cur->parent->adjacent[5]->children[curPos - 2];
                    }
                }
                if (curPos % 2 == 0) { // we are on the left side of the subdivision
                    if (cur->parent->adjacent[7] != nullptr) {
                        cur->adjacent[7] = cur->parent->adjacent[7]->children[curPos + 1];
                    }
                    cur->adjacent[3] = cur->parent->children[curPos + 1];
                } else { // we are on the right side of the subdivision
                    cur->adjacent[7] = cur->parent->children[curPos - 1];
                    if (cur->parent->adjacent[3] != nullptr) {
                        cur->adjacent[3] = cur->parent->adjacent[3]->children[curPos - 1];
                    }
                }
            }
        }
        // build diagonals using existing values for cardinal directions
        ptrQueue.push(s);
        while (!ptrQueue.empty()) {
            Subdivision* cur = ptrQueue.front();
            ptrQueue.pop();
            for (auto & c : cur->children) {
                if (c != nullptr) {
                    ptrQueue.push(c);
                }
            }
            if (cur->adjacent[1] != nullptr) {
                cur->adjacent[0] = cur->adjacent[1]->adjacent[7];
                cur->adjacent[2] = cur->adjacent[1]->adjacent[3];
            }
            if (cur->adjacent[5] != nullptr) {
                cur->adjacent[4] = cur->adjacent[5]->adjacent[3];
                cur->adjacent[6] = cur->adjacent[5]->adjacent[7];
            }
        }
    }
}

Simulation::Simulation()  {
    unsigned int subdivisionCount = 5; // how many times should we subdivide the grid
    Subdivision top(-WORLD_SIZE, WORLD_SIZE, WORLD_SIZE, -WORLD_SIZE, nullptr);
    subdivide(&top, subdivisionCount);
    buildAdjacency(&top);


//    for (int x = -100; x <= 100; x += 10) {
//        for (int y = -100; y <= 100; y += 10) {
//            objects.emplace_back(glm::vec2(x, y), 5);
//        }
//    }
}

std::vector<Simulation::PhysicsObject>* Simulation::step(float deltaTime) {
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