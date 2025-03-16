#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray() {}
    Ray(Vec3 origin_, Vec3 direction_)
        : origin(origin_), direction(direction_) {}

    Vec3 at(float time) const { return origin + direction * time; }
} Ray;

#endif
