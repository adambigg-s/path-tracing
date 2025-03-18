#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#include "utils.h"

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

inline Vec3 vec3_mul_component(Vec3 u, Vec3 v) {
    Vec3 vec = {.x = u.x * v.x, .y = u.y * v.y, .z = u.z * v.z};
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

inline bool vec3_near_zero(Vec3 *target) {
    float near = 1e-8;
    return (target->x < near) && (target->y < near) && (target->z < near);
}

inline Vec3 vec3_reflect(Vec3 *vec, Vec3 *normal) {
    float dot = 2 * vec3_inner_product(*vec, *normal);
    Vec3 term = vec3_mul(*normal, dot);

    return vec3_sub(*vec, term);
}

inline Vec3 vec3_refract(Vec3 *vec, Vec3 *normal, float ratio) {
    float cos_theta = fmin(1.0, vec3_inner_product(vec3_neg(*vec), *normal));

    Vec3 perp = vec3_mul(vec3_add(*vec, vec3_mul(*normal, cos_theta)), ratio);

    float parallel_scale = -sqrt(1 - vec3_inner_product(perp, perp));
    Vec3 parallel = vec3_mul(*normal, parallel_scale);

    return vec3_add(perp, parallel);
}

inline Vec3 vec3_random() {
    Vec3 vec = {
        .x = random_float(),
        .y = random_float(),
        .z = random_float(),
    };
    return vec;
}

inline Vec3 vec3_random_range(float min, float max) {
    Vec3 vec = {
        .x = random_range(min, max),
        .y = random_range(min, max),
        .z = random_range(min, max),
    };
    return vec;
}

inline Vec3 vec3_random_unit_vector() {
    while (true) {
        Vec3 candidate = vec3_random_range(-1, 1);
        float squared_length = vec3_inner_product(candidate, candidate);
        if (1e-30 < squared_length && squared_length <= 1) {
            return vec3_div(candidate, sqrt(squared_length));
        }
    }
}

inline Vec3 vec3_random_on_hemisphere(Vec3 *normal) {
    Vec3 on_unit = vec3_random_unit_vector();
    if (vec3_inner_product(on_unit, *normal) > 0) {
        return on_unit;
    } else {
        return vec3_neg(on_unit);
    }
}

#endif
