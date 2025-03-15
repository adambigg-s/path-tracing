#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int main() {
    int image_width = 1920;
    int image_height = 1080;

    FILE *output = NULL;
    errno_t err = fopen_s(&output, "./images/format_testing.ppm", "w");
    if (err != 0 || output == NULL) {
        perror("some error opening the file");
        return 1;
    }

    size_t buffer_size = image_width * 20;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        perror("buffer allc fail");
        fclose(output);
        return 1;
    }

    fprintf(output, "P3\n%d %d \n255\n", image_width, image_height);
    for (int j = 0; j < image_height; j += 1) {
        printf("scanlines remaining: %d\n", image_height - j);

        char *ptr = buffer;
        for (int i = 0; i < image_width; i += 1) {
            float r = (float)i / (image_width - 1);
            float g = (float)j / (image_height - 1);
            float b = 0.0;

            int ir = 255 * r;
            int ig = 255 * g;
            int ib = 255 * b;

            ptr += sprintf(ptr, "%d %d %d\n", ir, ig, ib);
        }

        fwrite(buffer, sizeof(char), ptr - buffer, output);
    }

    printf("done processing");

    return 0;
}
