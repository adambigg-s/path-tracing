#ifndef VEC3_H
#define VEC3_H

typedef struct Vec3 {
    float x;
    float y;
    float z;
} Vec3;

inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    Vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

#endif
