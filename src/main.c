#include <stdio.h>

#include "camera.h"
#include "pathtracer.h"

int main(int argc, char *argv[]) {
    Camera camera = camera_build(400, 400 * 9 / 16, 50);

    Material mat_ground = {.type = Lambertian, .albedo = vec3_build(0.2, 0.2, 0.2)};
    Material mat_center = {.type = Lambertian, .albedo = vec3_build(0.1, 0.2, 0.5)};
    Material mat_back = {.type = Metal, .albedo = vec3_build(0.3, 0.8, 0.3), .fuzzy = 0.1};
    Material mat_left = {.type = Metal, .albedo = vec3_build(0.8, 0.8, 0.8), .fuzzy = 0.0};
    Material mat_right = {.type = Metal, .albedo = vec3_build(0.8, 0.6, 0.2), .fuzzy = 0.5};
    Material mat_little = {.type = Metal, .albedo = vec3_build(0, 1, 1), .fuzzy = 0.2};

    SphereList scene = spherelist_new();
    spherelist_add(&scene, sphere_build(vec3_build(0, -400.5, -1), 400, &mat_ground));
    spherelist_add(&scene, sphere_build(vec3_build(0.1, 0, -1.5), 0.5, &mat_left));
    spherelist_add(&scene, sphere_build(vec3_build(-1.0, -0.2, -0.9), 0.3, &mat_center));
    spherelist_add(&scene, sphere_build(vec3_build(1.1, 0, -1.1), 0.5, &mat_right));
    spherelist_add(&scene, sphere_build(vec3_build(0.5, -0.4, -0.8), 0.1, &mat_little));
    spherelist_add(&scene, sphere_build(vec3_build(-1.5, 0, -3), 0.5, &mat_back));

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
