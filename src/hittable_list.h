#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable {
  public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<Hittable> object) { objects.push_back(object); }

    bool hit(const Ray &ray, Interval ray_time,
             HitRecord &record) const override {
        HitRecord temp_record;
        bool hit_anything = false;
        auto closest_so_far = ray_time.max;

        for (const auto &object : objects) {
            if (object->hit(ray, Interval(ray_time.min, closest_so_far),
                            temp_record)) {
                hit_anything = true;
                closest_so_far = temp_record.time;
                record = temp_record;
            }
        }

        return hit_anything;
    }
};

#endif
