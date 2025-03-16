#include "constants.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

Vec3 ray_color(const Ray &ray, const Hittable &world) {
    HitRecord record;
    if (world.hit(ray, Interval(0, INF), record)) {
        return (record.normal + Vec3(1, 1, 1)) * 0.5;
    }

    Vec3 unit_direction = ray.direction.normalized();
    auto alpha = 0.5 * (unit_direction.y + 1.);
    return Vec3(1, 1, 1) * (1 - alpha) + Vec3(0.5, 0.7, 1) * alpha;
}

int main() {
    auto aspect_ratio = 16. / 9.;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);

    HittableList world;
    world.add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

    auto focal_length = 1.;
    auto viewport_height = 2.;
    auto viewport_width =
        viewport_height * float(image_width) / float(image_height);
    auto camera_center = Vec3(0, 0, 0);

    Vec3 viewport_u = Vec3(viewport_width, 0, 0);
    Vec3 viewport_v = Vec3(0, -viewport_height, 0);

    Vec3 pixel_delta_u = viewport_u / image_width;
    Vec3 pixel_delta_v = viewport_v / image_height;

    Vec3 viewport_upper_left = camera_center - Vec3(0, 0, focal_length) -
                               viewport_u / 2 - viewport_v / 2;

    Vec3 pixel00_loc =
        viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j += 1) {

        std::clog << "scanlines remaining: " << (image_height - j) << '\n'
                  << std::flush;

        for (int i = 0; i < image_width; i += 1) {
            auto pixel_center =
                pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j);
            auto ray_direction = pixel_center - camera_center;

            Ray ray(camera_center, ray_direction);

            Vec3 pixel_color = ray_color(ray, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "finished processing image\n";

    return 0;
}
