#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

class HitRecord {
  public:
    Vec3 point;
    Vec3 normal;
    float time;
    bool front_face;

    void set_face_normal(const Ray &ray, const Vec3 &outward_normal) {
        front_face = ray.direction.inner_product(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, Interval ray_time,
                     HitRecord &record) const = 0;
};

#endif
