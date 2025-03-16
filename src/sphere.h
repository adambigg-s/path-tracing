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

    bool hit(const Ray &ray, Interval ray_time,
             HitRecord &record) const override {
        Vec3 oc = center - ray.origin;
        auto direction_squared = ray.direction.inner_product(ray.direction);
        auto correlation = ray.direction.inner_product(oc);
        auto difference = oc.inner_product(oc) - radius * radius;
        auto discrim =
            correlation * correlation - direction_squared * difference;

        if (discrim < 0) {
            return false;
        }

        auto sqrtd = sqrt(discrim);

        auto root = (correlation - sqrtd) / direction_squared;

        if (!ray_time.surrounds(root)) {
            root = (correlation + sqrtd) / direction_squared;
            if (!ray_time.surrounds(root)) {
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
