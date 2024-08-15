#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Kalev Martinson on 8/15/24.
//

#ifndef GRAVITYSIMULATOR_SUBDIVISION_H
#define GRAVITYSIMULATOR_SUBDIVISION_H

#include <queue>
#include "PhysicsObject.h"

struct Subdivision {
    float left;
    float right;
    float up;
    float down;
    Subdivision* parent;
    Subdivision* children[4]; // in order: top left, top right, bottom left, bottom right
    Subdivision* adjacent[8]; // in order, top left then going clockwise

    std::vector<PhysicsObject*> objectsContained;

    void clearObjects() {
        objectsContained.clear();
        for (auto & c : children) {
            if (c != nullptr) {
                c->clearObjects();
            }
        }
    }

    void placeObject(PhysicsObject* obj) {
        objectsContained.push_back(obj);
        for (auto c : children) {
            if (c != nullptr
            && c->left <= obj->position.x
            && obj->position.x <= c->right
            && c->down <= obj->position.y
            && obj->position.y <= c->up) {
                c->placeObject(obj);
                break;
            }
        }
    }

    Subdivision(float left, float right, float up, float down, Subdivision* parent) :
            left(left), right(right), up(up), down(down), parent(parent), children(), adjacent(), objectsContained() {}

    ~Subdivision() {
        for (auto & c : children) {
            delete c;
        }
    }
    static void subdivide(Subdivision* s, unsigned int depthRemaining) {
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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LocalValueEscapesScope"
    static void buildAdjacency(Subdivision* s) {
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
#pragma clang diagnostic pop
};

#endif //GRAVITYSIMULATOR_SUBDIVISION_H
#pragma clang diagnostic pop