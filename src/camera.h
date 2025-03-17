#ifndef CAMERA_H
#define CAMERA_H

#include "pathtracer.h"
#include "utils.h"
#include <float.h>

typedef struct Camera {
    float aspect_ratio;
    int width;
    int height;
    Vec3 center;
    Vec3 pixel_top_left;
    Vec3 pixel_du;
    Vec3 pixel_dv;
    int samples_per_pixel;
} Camera;

inline Camera camera_build(int width, int height, int samples) {
    float aspect_ratio = (float)width / (float)height;
    float focal_length = 1;
    float viewport_height = 2;
    float viewport_width = viewport_height * aspect_ratio;
    Vec3 viewport_u = vec3_build(viewport_width, 0, 0);
    Vec3 viewport_v = vec3_build(0, -viewport_height, 0);
    Vec3 camera_center = vec3_build(0, 0, 0);

    Vec3 pixel_du = vec3_div(viewport_u, width);
    Vec3 pixel_dv = vec3_div(viewport_v, height);

    Vec3 viewport_top_left =
        vec3_sub(vec3_sub(vec3_sub(camera_center, vec3_build(0, 0, focal_length)), vec3_div(viewport_u, 2)),
            vec3_div(viewport_v, 2));
    Vec3 pixel_top_left = vec3_add(viewport_top_left, vec3_mul(vec3_add(pixel_du, pixel_dv), 0.5));

    Camera out = {
        .aspect_ratio = aspect_ratio,
        .width = width,
        .height = height,
        .center = camera_center,
        .pixel_top_left = pixel_top_left,
        .pixel_du = pixel_du,
        .pixel_dv = pixel_dv,
        .samples_per_pixel = samples,
    };
    return out;
}

inline Vec3 sample_square() { return vec3_build(random_float() - 0.5, random_float() - 0.5, 0); }

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
    int printerval = camera->width * camera->height / 10000;
    for (int j = 0; j < camera->height; j += 1) {
        for (int i = 0; i < camera->width; i += 1) {
            Vec3 pixel_color = vec3_build(0, 0, 0);
            for (int sample = 0; sample < camera->samples_per_pixel; sample += 1) {
                Ray ray = get_ray(camera, i, j);
                Vec3 contribution = ray_color(&ray, scene);
                pixel_color = vec3_add(pixel_color, contribution);
            }
            pixel_color = vec3_div(pixel_color, camera->samples_per_pixel);
            color_write(pixel_color, file);
        }
        fprintf(file, "\n");
        if (j % printerval == 0) {
            printf("scanlines progress: %.2f\n", (float)j / camera->height * 100);
        }
    }
}

#endif
