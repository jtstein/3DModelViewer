
#ifndef PARTICLE_H
#define PARTICLE_H

#include <Vector3.h>
#include "Color3.h"

struct Particle {
    Vector3f position;
    Vector3f velocity;
    Vector3f force;
    Color3f color;
    float mass;
    float lifetime;
};

#endif
