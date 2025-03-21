#include "pathtracer.h"

Ray ray_build(Vec3 origin, Vec3 direction) {
    Ray ray = {.origin = origin, .direction = direction};
    return ray;
}

Vec3 ray_at(Ray *ray, float time) {
    Vec3 delta = vec3_mul(ray->direction, time);
    return vec3_add(ray->origin, delta);
}

HitRecord hitrecord_new() {
    HitRecord out = {.hit = false, .front_face = false, .time = 0, .normal = vec3_build(1, 1, 1)};
    return out;
}

void hitrecord_set_normal(HitRecord *record, Ray *ray, Vec3 outward_normal) {
    record->front_face = vec3_inner_product(ray->direction, outward_normal) < 0;
    record->normal = record->front_face ? outward_normal : vec3_neg(outward_normal);
}
