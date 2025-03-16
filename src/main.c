#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

typedef Vec3 Point3;
typedef Vec3 Color;

void color_write(Color pixel_color, FILE *file) {
    int red = (int)(255.999 * pixel_color.x);
    int green = (int)(255.999 * pixel_color.y);
    int blue = (int)(255.999 * pixel_color.z);
    fprintf(file, "%d %d %d ", red, green, blue);
}

int main(int argc, char *argv[]) {
    int width = 1920;
    int height = 1080;

    const char *output_file = (argc > 1) ? argv[1] : "./images/dump.ppm";
    FILE *file;
    errno_t error = fopen_s(&file, output_file, "w");
    if (error != 0 || file == NULL) {
        printf("error opening image file");
        return 1;
    }

    fprintf(file, "P3\n%d %d\n255\n", width, height);
    for (int j = 0; j < height; j += 1) {
        if (j % 15 == 0) {
            printf("scanlines progress: %.2f\n", (float)j / height * 100);
        }
        for (int i = 0; i < width; i += 1) {
            Color pixel_color =
                vec3_build((float)i / width, (float)j / height, (float)(i * j) / (width * height));
            color_write(pixel_color, file);
        }
        fprintf(file, "\n");
    }
    printf("image done processing. target: %s", output_file);

    fclose(file);

    return 0;
}
