#include <stdbool.h>

#include "pathtracer.h"
#include "utils.h"
#include "vector.h"

float material_reflectance(float cosine, float refraction_index) {
    float ref = (1 - refraction_index) / (1 + refraction_index);
    ref = ref * ref;

    return ref + (1 - ref) * pow((1 - cosine), 5);
}

bool material_scatter(Material *material, Ray *ray_in, HitRecord *record, Vec3 *attenuation, Ray *scattered) {
    if (material->type == Lambertian) {
        Vec3 scatter_direction = vec3_add(record->normal, vec3_random_unit_vector());
        if (vec3_near_zero(&scatter_direction)) {
            scatter_direction = record->normal;
        }
        *scattered = ray_build(record->point, scatter_direction);
        *attenuation = material->albedo;

        return true;
    } else if (material->type == Metal) {
        Vec3 fuzz_factor = vec3_mul(vec3_random_unit_vector(), material->fuzzy);
        Vec3 reflected = vec3_reflect(&ray_in->direction, &record->normal);
        reflected = vec3_normalized(reflected);
        reflected = vec3_add(reflected, fuzz_factor);
        *scattered = ray_build(record->point, reflected);
        *attenuation = material->albedo;

        return (vec3_inner_product(scattered->direction, record->normal) > 0);
    } else if (material->type == Dielectric) {
        float refraction_index =
            record->front_face ? (1. / material->refraction_index) : material->refraction_index;

        Vec3 unit_direction = vec3_normalized(ray_in->direction);
        float cos_theta = fmin(vec3_inner_product(vec3_neg(unit_direction), record->normal), 1);
        float sin_theta = sqrt(1 - cos_theta * cos_theta);

        Vec3 direction;
        if ((refraction_index * sin_theta > 1) ||
            (material_reflectance(cos_theta, refraction_index) > random_float())) {
            direction = vec3_reflect(&unit_direction, &record->normal);
        } else {
            direction = vec3_refract(&unit_direction, &record->normal, refraction_index);
        }

        *scattered = ray_build(record->point, direction);
        *attenuation = vec3_build(1, 1, 1);

        return true;
    } else if (material->type == Source) {
        *attenuation = vec3_mul(material->albedo, material->source_strength);

        return false;
    } else if (material->type == SemiVolumetric) {
    }

    return false;
}

Sphere sphere_build(Vec3 center, float radius, Material *material) {
    Sphere out = {.center = center, .radius = radius, .material = material};
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
        root = (eta + square_root) / alpha;
        if (!interval_contains(&ray_time, root)) {
            return false;
        }
    }

    record->time = root;
    record->point = ray_at(ray, root);
    Vec3 outward_normal = vec3_div(vec3_sub(record->point, sphere->center), sphere->radius);
    hitrecord_set_normal(record, ray, outward_normal);
    record->hit = true;
    record->material = sphere->material;

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
