#include <time.h>

#include "camera.h"
#include "pathtracer.h"
#include "vector.h"

int main(int argc, char *argv[]) {
    Camera camera = camera_build_default(2560, 1440, 5000);

    Material ground = {.type = Lambertian, .albedo = vec3_build(0.2, 0.2, 0.2)};
    Material small = {.type = Lambertian, .albedo = vec3_build(0.7, 0.1, 0.1)};
    Material tiny = {.type = Lambertian, .albedo = vec3_build(0.89, 0.55, 0.94)};
    Material left = {.type = Dielectric, .refraction_index = 1.4};
    Material bubble = {.type = Dielectric, .refraction_index = 1. / 1.4};
    Material big_outer = {.type = Dielectric, .refraction_index = 1.7};
    Material dark = {.type = Metal, .albedo = vec3_build(0.05, 0.05, 0.05), .fuzzy = 0.85};
    Material back = {.type = Metal, .albedo = vec3_build(0.3, 0.8, 0.3), .fuzzy = 0.25};
    Material center = {.type = Metal, .albedo = vec3_build(0.5, 0.5, 0.5), .fuzzy = 0.0};
    Material right = {.type = Metal, .albedo = vec3_build(0.8, 0.6, 0.2), .fuzzy = 0.25};
    Material little = {.type = Metal, .albedo = vec3_build(0, 1, 1), .fuzzy = 0.2};
    Material big_inner = {.type = Metal, .albedo = vec3_build(0.9, 0.5, 0.2), .fuzzy = 0.02};
    Material mirror = {.type = Metal, .albedo = vec3_build(1, 1, 1), .fuzzy = 0};
    Material source = {.type = Source, .albedo = vec3_build(1, 1, 1), .source_strength = 40};
    Material dim_light = {.type = Source, .albedo = vec3_build(1, 0.7, 0.5), .source_strength = 5};
    Material red_light = {.type = Source, .albedo = vec3_build(1, 0.05, 0.05), .source_strength = 5};
    Material blue_light = {.type = Source, .albedo = vec3_build(0, 0.9, 1), .source_strength = 7};
    Material light_green = {.type = Source, .albedo = vec3_build(0.1, 1, 0.1), .source_strength = 7};

    SphereList scene = spherelist_new();
    spherelist_add(&scene, sphere_build(vec3_build(0, -300.5, -1), 300, &ground));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, 0, -1.5), 0.5, &center));
    spherelist_add(&scene, sphere_build(vec3_build(-1.0, -0.2, -0.9), 0.3, &left));
    spherelist_add(&scene, sphere_build(vec3_build(-1.0, -0.2, -0.9), 0.15, &bubble));
    spherelist_add(&scene, sphere_build(vec3_build(1.1, 0, -1.1), 0.5, &right));
    spherelist_add(&scene, sphere_build(vec3_build(0.5, -0.4, -0.75), 0.1, &little));
    spherelist_add(&scene, sphere_build(vec3_build(-0.6, -0.4, -1.2), 0.1, &small));
    spherelist_add(&scene, sphere_build(vec3_build(-0.6, -0.35, -0.4), 0.15, &dark));
    spherelist_add(&scene, sphere_build(vec3_build(-0.55, -0.4, -1.8), 0.1, &source));
    spherelist_add(&scene, sphere_build(vec3_build(-0.1, -0.45, -0.5), 0.05, &tiny));
    spherelist_add(&scene, sphere_build(vec3_build(0.4, -0.47, -0.12), 0.03, &tiny));
    spherelist_add(&scene, sphere_build(vec3_build(-1.5, 0, -3), 0.5, &back));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 1, -4), 1.5, &big_outer));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 1, -4), 0.75, &big_inner));
    spherelist_add(&scene, sphere_build(vec3_build(0.7, -0.3, 0), 0.2, &mirror));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, 0, 2), 0.5, &center));
    spherelist_add(&scene, sphere_build(vec3_build(-1, 0, 1.5), 0.5, &right));
    spherelist_add(&scene, sphere_build(vec3_build(-4, 0, 0), 0.5, &little));
    spherelist_add(&scene, sphere_build(vec3_build(10, 8, -1), 5, &source));
    spherelist_add(&scene, sphere_build(vec3_build(-0.2, -0.4, 0.1), 0.1, &dim_light));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, -0.45, -0.05), 0.05, &big_outer));
    spherelist_add(&scene, sphere_build(vec3_build(-0.7, -0.4, -0.05), 0.1, &big_outer));
    spherelist_add(&scene, sphere_build(vec3_build(2.5, -0.2, -1.5), 0.3, &red_light));
    spherelist_add(&scene, sphere_build(vec3_build(-1.3, -0.45, -0.5), 0.05, &blue_light));
    spherelist_add(&scene, sphere_build(vec3_build(1, -0.3, 0.8), 0.2, &light_green));

    char *output_file = (argc > 1) ? argv[1] : "./images/dump.ppm";
    FILE *file;
    errno_t error = fopen_s(&file, output_file, "w");
    if (error != 0 || file == NULL) {
        printf("error opening image file");
        return 1;
    }

    clock_t start_time = clock();
    camera_render(&camera, &scene, file);
    clock_t end_time = clock();
    float time_spent_seconds = (float)(end_time - start_time) / CLOCKS_PER_SEC;
    float time_spent_minutes = (float)(end_time - start_time) / CLOCKS_PER_SEC / 60;
    printf("image done processing. target: %s\n", output_file);
    printf("took: %.3f seconds\n", time_spent_seconds);
    printf("took: %.3f minutes\n", time_spent_minutes);

    fclose(file);

    return 0;
}
