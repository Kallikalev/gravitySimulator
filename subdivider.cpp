#include <iostream>

//
// Created by Kalev Martinson on 8/12/24.
//

struct subdivision {
    float left;
    float right;
    float up;
    float down;
    subdivision* parent;
    subdivision* children[4];

    subdivision(float left, float right, float up, float down, subdivision* parent) : left(left), right(right),
        up(up), down(down), parent(parent), children() {}

    ~subdivision() {
        for (auto & i : children) {
            delete i;
        }
    }
};

void subdivide(subdivision* s, unsigned int depthRemaining) {
    // in order: top left, top right, bottom left, bottom right
    s->children[0] = new subdivision(s->left,(s->left + s->right)/2,s->up,(s->up + s->down)/2,s);
    s->children[1] = new subdivision((s->left + s->right)/2, s->right ,s->up,(s->up + s->down)/2,s);
    s->children[2] = new subdivision(s->left,(s->left + s->right)/2,(s->up + s->down)/2, s->down, s);
    s->children[3] = new subdivision((s->left + s->right)/2, s->right ,(s->up + s->down)/2, s->down,s);
    if (depthRemaining > 0) {
        for (auto & i : s->children) {
            subdivide(i,depthRemaining-1);
        }
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    // our grid goes from (-gridRadius,-gridRadius) on the
    // bottom-left to (gridRadius,gridRadius) on the top-right
    float gridRadius = 100;
    unsigned int subdivisionCount = 2; // how many times should we subdivide the grid
    subdivision top(-gridRadius,gridRadius,-gridRadius, gridRadius, nullptr);
    subdivide(&top, subdivisionCount);
    return 0;
}