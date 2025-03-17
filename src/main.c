#include <stdio.h>

#include "camera.h"
#include "pathtracer.h"

int main(int argc, char *argv[]) {
    Camera camera = camera_build(500, 500, 30);

    SphereList scene = spherelist_new();
    spherelist_add(&scene, sphere_build(vec3_build(0, 0, -1), 0.5));
    spherelist_add(&scene, sphere_build(vec3_build(0, -100.5, -1), 100));

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
