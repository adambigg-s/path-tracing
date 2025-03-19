#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <stdbool.h>

#include "vector.h"

#define MAX_HITTABLES 32

enum MaterialType {
    Lambertian,
    Metal,
    Dielectric,
    Source,
};

typedef struct Material {
    enum MaterialType type;
    Vec3 albedo;
    float fuzzy;
    float refraction_index;
    float source_strength;
} Material;

typedef struct Ray {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct HitRecord {
    Vec3 point;
    Vec3 normal;
    float time;
    bool hit;
    bool front_face;
    Material *material;
} HitRecord;

typedef struct Sphere {
    Vec3 center;
    float radius;
    Material *material;
} Sphere;

typedef struct SphereList {
    Sphere spheres[MAX_HITTABLES];
    int count;
} SphereList;

bool material_scatter(Material *material, Ray *ray_in, HitRecord *record, Vec3 *attenuation, Ray *scattered);

Ray ray_build(Vec3 origin, Vec3 direction);
Vec3 ray_at(Ray *ray, float time);

HitRecord hitrecord_new();
void hitrecord_set_normal(HitRecord *record, Ray *ray, Vec3 outward_normal);

Sphere sphere_build(Vec3 center, float radius, Material *material);
bool sphere_hit(Sphere *sphere, Ray *ray, Interval ray_time, HitRecord *recrod);

SphereList spherelist_new();
bool spherelist_add(SphereList *list, Sphere sphere);
bool spherelist_hit(SphereList *list, Ray *ray, Interval ray_time, HitRecord *record);

#endif
