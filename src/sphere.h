#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
  public:
    Vec3 center;
    float radius;

    Sphere(const Vec3 &center, float radius)
        : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const Ray& ray, float ray_time_min, float ray_time_max, HitRecord& record) const override {
        Vec3 oc = center - ray.origin;
        auto a = ray.direction.inner_product(ray.direction);
        auto h = ray.direction.inner_product(oc);
        auto c = oc.inner_product(oc) - radius * radius;
        auto discrim = h * h - a * c;

        if (discrim < 0) {
            return false;
        }

        auto sqrtd = sqrt(discrim);

        auto root = (h - sqrtd) / a;

        if (root <= ray_time_min || ray_time_max <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_time_min || ray_time_max <= root) {
                return false;
            }
        }

        record.time = root;
        record.point = ray.at(root);
        Vec3 outward_normal = (record.point - center) / radius;
        record.set_face_normal(ray, outward_normal);

        return true;
    }
};

#endif
