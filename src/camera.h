#ifndef CAMERA_H
#define CAMERA_H

#include <float.h>

#include "pathtracer.h"
#include "utils.h"
#include "vector.h"

typedef struct Camera {
    float aspect_ratio;
    int width;
    int height;
    float fov;
    Vec3 center;
    Vec3 pixel_top_left;
    Vec3 pixel_du;
    Vec3 pixel_dv;
    int samples_per_pixel;
    int max_recursive_depth;
} Camera;

inline Camera camera_build(int width, int height, int samples) {
    float fov = 70;
    Vec3 camera_center = vec3_build(0, 0.1, 1);
    Vec3 camera_lookat = vec3_build(0, 0, -1);
    Vec3 world_up = vec3_build(0, 1, 0);

    float aspect_ratio = (float)width / (float)height;
    float focal_length = vec3_length(vec3_sub(camera_center, camera_lookat));
    float theta = deg_to_rad(fov);
    float height_modifier = tan(theta / 2);
    float viewport_height = 2 * height_modifier * focal_length;
    float viewport_width = viewport_height * aspect_ratio;

    Vec3 w = vec3_normalized(vec3_sub(camera_center, camera_lookat));
    Vec3 u = vec3_normalized(vec3_cross_product(world_up, w));
    Vec3 v = vec3_cross_product(w, u);

    Vec3 viewport_u = vec3_mul(u, viewport_width);
    Vec3 viewport_v = vec3_mul(vec3_neg(v), viewport_height);

    Vec3 pixel_du = vec3_div(viewport_u, width);
    Vec3 pixel_dv = vec3_div(viewport_v, height);

    Vec3 viewport_top_left =
        vec3_sub(vec3_sub(vec3_sub(camera_center, vec3_mul_component(vec3_build(0, 0, focal_length), w)),
                     vec3_div(viewport_u, 2)),
            vec3_div(viewport_v, 2));
    Vec3 pixel_top_left = vec3_add(viewport_top_left, vec3_mul(vec3_add(pixel_du, pixel_dv), 0.5));

    Camera out = {
        .aspect_ratio = aspect_ratio,
        .width = width,
        .height = height,
        .fov = fov,
        .center = camera_center,
        .pixel_top_left = pixel_top_left,
        .pixel_du = pixel_du,
        .pixel_dv = pixel_dv,
        .samples_per_pixel = samples,
        .max_recursive_depth = 50,
    };
    return out;
}

inline void color_write(Vec3 pixel_color, FILE *file) {
    Interval interval = interval_build(0.000, 0.999);

    float red = linear_to_gamma(pixel_color.x);
    float green = linear_to_gamma(pixel_color.y);
    float blue = linear_to_gamma(pixel_color.z);

    int red_byte = (int)(256 * interval_clamp(&interval, red));
    int green_byte = (int)(256 * interval_clamp(&interval, green));
    int blue_byte = (int)(256 * interval_clamp(&interval, blue));

    fprintf(file, "%d %d %d ", red_byte, green_byte, blue_byte);
}

inline Vec3 sample_square() { return vec3_build(random_float() - 0.5, random_float() - 0.5, 0); }

inline Vec3 ray_color(Ray *ray, SphereList *scene, int depth) {
    if (depth < 0) {
        return vec3_build(0, 1, 1);
    }
    HitRecord record = hitrecord_new();
    if (spherelist_hit(scene, ray, interval_build(0.001, INFIN), &record)) {
        Ray scattered;
        Vec3 attenuation;
        if (material_scatter(record.material, ray, &record, &attenuation, &scattered)) {
            return vec3_mul_component(attenuation, ray_color(&scattered, scene, depth - 1));
        }
        return vec3_build(0, 0, 0);
    }

    Vec3 unit_direction = vec3_normalized(ray->direction);
    float alpha = 0.5 * (unit_direction.y + 1);
    Vec3 term1 = vec3_mul(vec3_build(1, 1, 1), (1 - alpha));
    Vec3 term2 = vec3_mul(vec3_build(0.4, 0.6, 0.9), alpha);
    return vec3_add(term1, term2);
}

inline Ray get_ray(Camera *camera, int i, int j) {
    Vec3 pixel_center = camera->pixel_top_left;
    pixel_center = vec3_add(pixel_center, vec3_mul(camera->pixel_du, i));
    pixel_center = vec3_add(pixel_center, vec3_mul(camera->pixel_dv, j));

    Vec3 offset = sample_square();
    Vec3 pixel_sample = pixel_center;
    pixel_sample = vec3_add(pixel_sample, vec3_mul(camera->pixel_du, offset.x));
    pixel_sample = vec3_add(pixel_sample, vec3_mul(camera->pixel_dv, offset.y));

    Vec3 ray_direction = vec3_sub(pixel_sample, camera->center);

    return ray_build(camera->center, ray_direction);
}

inline void camera_render(Camera *camera, SphereList *scene, FILE *file) {
    fprintf(file, "P3\n%d %d\n255\n", camera->width, camera->height);
    int printerval = camera->height / 50;
    for (int j = 0; j < camera->height; j += 1) {
        for (int i = 0; i < camera->width; i += 1) {
            Vec3 pixel_color = vec3_build(0, 0, 0);
            for (int sample = 0; sample < camera->samples_per_pixel; sample += 1) {
                Ray ray = get_ray(camera, i, j);
                Vec3 contribution = ray_color(&ray, scene, camera->max_recursive_depth);
                pixel_color = vec3_add(pixel_color, contribution);
            }
            pixel_color = vec3_div(pixel_color, camera->samples_per_pixel);
            color_write(pixel_color, file);
        }
        fprintf(file, "\n");
        if (j % printerval == 0) {
            printf("progress: %.2f\n", (float)j / camera->height * 100);
        }
    }
}

#endif
