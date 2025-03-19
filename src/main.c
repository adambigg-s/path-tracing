#include <stdio.h>

#include "camera.h"
#include "pathtracer.h"

int main(int argc, char *argv[]) {
    Camera camera = camera_build(2560, 1440, 1500);

    Material mat_ground = {.type = Lambertian, .albedo = vec3_build(0.2, 0.2, 0.2), .fuzzy = 0.7};
    Material mat_left = {.type = Dielectric, .refraction_index = 1.4};
    Material mat_bubble = {.type = Dielectric, .refraction_index = 1. / 1.4};
    Material mat_back = {.type = Metal, .albedo = vec3_build(0.3, 0.8, 0.3), .fuzzy = 0.1};
    Material mat_center = {.type = Metal, .albedo = vec3_build(0.5, 0.5, 0.5), .fuzzy = 0.0};
    Material mat_right = {.type = Metal, .albedo = vec3_build(0.8, 0.6, 0.2), .fuzzy = 0.25};
    Material mat_little = {.type = Metal, .albedo = vec3_build(0, 1, 1), .fuzzy = 0.2};
    Material mat_small = {.type = Lambertian, .albedo = vec3_build(0.7, 0.1, 0.1)};
    Material mat_large_outer = {.type = Dielectric, .refraction_index = 1.7};
    Material mat_large_inner = {.type = Metal, .albedo = vec3_build(0.9, 0.5, 0.2), .fuzzy = 0.05};
    Material mirror = {.type = Metal, .albedo = vec3_build(1, 1, 1), .fuzzy = 0};
    Material source = {.type = Source, .albedo = vec3_build(1, 0.9, 0.8), .source_strength = 10};
    Material dim_light = {.type = Source, .albedo = vec3_build(1, 0.7, 0.5), .source_strength = 5};
    Material light_red = {.type = Source, .albedo = vec3_build(1, 0.2, 0.2), .source_strength = 4};
    Material light_blue = {.type = Source, .albedo = vec3_build(0, 0.9, 1), .source_strength = 4};

    SphereList scene = spherelist_new();
    spherelist_add(&scene, sphere_build(vec3_build(0, -400.5, -1), 400, &mat_ground));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, 0, -1.5), 0.5, &mat_center));
    spherelist_add(&scene, sphere_build(vec3_build(-1.0, -0.2, -0.9), 0.3, &mat_left));
    spherelist_add(&scene, sphere_build(vec3_build(-1.0, -0.2, -0.9), 0.15, &mat_bubble));
    spherelist_add(&scene, sphere_build(vec3_build(1.1, 0, -1.1), 0.5, &mat_right));
    spherelist_add(&scene, sphere_build(vec3_build(0.5, -0.4, -0.8), 0.1, &mat_little));
    spherelist_add(&scene, sphere_build(vec3_build(-0.6, -0.4, -1.2), 0.1, &mat_small));
    spherelist_add(&scene, sphere_build(vec3_build(-1.5, 0, -3), 0.5, &mat_back));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 1, -4), 1.5, &mat_large_outer));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 1, -4), 0.75, &mat_large_inner));
    spherelist_add(&scene, sphere_build(vec3_build(0.7, -0.3, 0), 0.2, &mirror));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, 0, 2), 0.5, &mat_center));
    spherelist_add(&scene, sphere_build(vec3_build(-1, 0, 1.5), 0.5, &mat_right));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 0, 0), 0.5, &mat_little));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 4, -1), 1, &source));
    spherelist_add(&scene, sphere_build(vec3_build(-0.2, -0.4, 0.1), 0.1, &dim_light));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, -0.45, -0.05), 0.05, &mat_large_outer));
    spherelist_add(&scene, sphere_build(vec3_build(-0.7, -0.4, -0.05), 0.1, &mat_large_outer));
    spherelist_add(&scene, sphere_build(vec3_build(2.5, -0.2, -1.5), 0.3, &light_red));
    spherelist_add(&scene, sphere_build(vec3_build(-1.3, -0.45, -0.5), 0.05, &light_blue));

    char *output_file = (argc > 1) ? argv[1] : "./images/dump.ppm";
    FILE *file;
    errno_t error = fopen_s(&file, output_file, "w");
    if (error != 0 || file == NULL) {
        printf("error opening image file");
        return 1;
    }

    camera_render(&camera, &scene, file);
    printf("image done processing. target: %s", output_file);

    fclose(file);

    return 0;
}
