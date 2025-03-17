#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <stdbool.h>

#include "utils.h"
#include "vector.h"

#define MAX_HITTABLES 16

typedef struct Ray {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct HitRecord {
    Vec3 point;
    Vec3 normal;
    float time;
    bool hit;
} HitRecord;

typedef struct Sphere {
    Vec3 center;
    float radius;
} Sphere;

typedef struct SphereList {
    Sphere spheres[MAX_HITTABLES];
    int count;
} SphereList;

Ray ray_build(Vec3 origin, Vec3 direction);
Vec3 ray_at(Ray *ray, float time);
Vec3 ray_color(Ray *ray, SphereList *scene);

HitRecord hitrecord_new();
void hitrecord_set_normal(HitRecord *record, Ray *ray, Vec3 outward_normal);

Sphere sphere_build(Vec3 center, float radius);
bool sphere_hit(Sphere *sphere, Ray *ray, Interval ray_time, HitRecord *recrod);

SphereList spherelist_new();
bool spherelist_add(SphereList *list, Sphere sphere);
bool spherelist_hit(SphereList *list, Ray *ray, Interval ray_time, HitRecord *record);

#endif
