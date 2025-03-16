#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

inline Vec3 vec3_build(float x, float y, float z) {
    Vec3 vec = {.x = x, .y = y, .z = z};
    return vec;
}

inline Vec3 vec3_neg(Vec3 target) {
    Vec3 vec = {.x = -target.x, .y = -target.y, .z = -target.z};
    return vec;
}

inline Vec3 vec3_add(Vec3 u, Vec3 v) {
    Vec3 vec = {.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
    return vec;
}

inline Vec3 vec3_sub(Vec3 u, Vec3 v) {
    Vec3 vec = {.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
    return vec;
}

inline Vec3 vec3_mul(Vec3 target, float scalar) {
    Vec3 vec = {.x = target.x * scalar, .y = target.y * scalar, .z = target.z * scalar};
    return vec;
}

inline Vec3 vec3_div(Vec3 target, float scalar) {
    Vec3 vec = {.x = target.x / scalar, .y = target.y / scalar, .z = target.z / scalar};
    return vec;
}

inline float vec3_inner_product(Vec3 u, Vec3 v) {
    float out = u.x * v.x + u.y * v.y + u.z * v.z;
    return out;
}

inline Vec3 vec3_cross_product(Vec3 u, Vec3 v) {
    Vec3 vec = {
        .x = u.y * v.z - u.z * v.y,
        .y = u.z * v.x - u.x * u.z,
        .z = u.x * v.y - u.y * v.x,
    };
    return vec;
}

inline float vec3_length(Vec3 target) {
    float out = sqrt(vec3_inner_product(target, target));
    return out;
}

inline Vec3 vec3_normalized(Vec3 target) {
    float length = vec3_length(target);
    return vec3_div(target, length);
}

#endif
