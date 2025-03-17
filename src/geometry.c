#include <stdbool.h>

#include "pathtracer.h"
#include "utils.h"

Ray ray_build(Vec3 origin, Vec3 direction) {
    Ray ray = {.origin = origin, .direction = direction};
    return ray;
}

Vec3 ray_at(Ray *ray, float time) {
    Vec3 delta = vec3_mul(ray->direction, time);
    return vec3_add(ray->origin, delta);
}

Vec3 ray_color(Ray *ray, SphereList *scene) {
    HitRecord record = hitrecord_new();
    if (spherelist_hit(scene, ray, interval_build(0, INFIN), &record)) {
        Vec3 normal = record.normal;
        return vec3_mul(vec3_build(normal.x + 1, normal.y + 1, normal.z + 1), 0.5);
    }

    Vec3 unit_direction = vec3_normalized(ray->direction);
    float alpha = 0.5 * (unit_direction.y + 1);
    Vec3 t1 = vec3_mul(vec3_build(1, 1, 1), (1 - alpha));
    Vec3 t2 = vec3_mul(vec3_build(0.5, 0.7, 1), alpha);
    return vec3_add(t1, t2);
}

HitRecord hitrecord_new() {
    HitRecord out = {.hit = false};
    return out;
}

void hitrecord_set_normal(HitRecord *record, Ray *ray, Vec3 outward_normal) {
    bool front_face = vec3_inner_product(ray->direction, outward_normal) < 0;
    record->normal = front_face ? outward_normal : vec3_neg(outward_normal);
}

Sphere sphere_build(Vec3 center, float radius) {
    Sphere out = {.center = center, .radius = radius};
    return out;
}

bool sphere_hit(Sphere *sphere, Ray *ray, Interval ray_time, HitRecord *record) {
    Vec3 rel_center = vec3_sub(sphere->center, ray->origin);
    float alpha = vec3_inner_product(ray->direction, ray->direction);
    float eta = vec3_inner_product(ray->direction, rel_center);
    float gamma = vec3_inner_product(rel_center, rel_center) - sphere->radius * sphere->radius;

    float discriminant = eta * eta - alpha * gamma;
    if (discriminant < 0) {
        return false;
    }

    float square_root = sqrt(discriminant);
    float root = (eta - square_root) / alpha;
    if (!interval_contains(&ray_time, root)) {
        root = (-gamma + square_root) / alpha;
        if (!interval_contains(&ray_time, root)) {
            return false;
        }
    }

    record->time = root;
    record->point = ray_at(ray, root);
    Vec3 outward_normal = vec3_div(vec3_sub(record->point, sphere->center), sphere->radius);
    hitrecord_set_normal(record, ray, outward_normal);
    record->hit = true;
    return true;
}

SphereList spherelist_new() {
    SphereList list = {.count = 0};
    return list;
}

bool spherelist_add(SphereList *list, Sphere sphere) {
    if (list->count >= MAX_HITTABLES) {
        return false;
    }
    list->spheres[list->count] = sphere;
    list->count += 1;
    return true;
}

bool spherelist_hit(SphereList *list, Ray *ray, Interval ray_time, HitRecord *record) {
    HitRecord temp_record = hitrecord_new();
    bool hit_anything = false;
    float closest = ray_time.max;

    for (int i = 0; i < list->count; i += 1) {
        if (sphere_hit(&list->spheres[i], ray, interval_build(ray_time.min, closest), &temp_record)) {
            hit_anything = true;
            closest = temp_record.time;
            *record = temp_record;
        }
    }

    return hit_anything;
}
