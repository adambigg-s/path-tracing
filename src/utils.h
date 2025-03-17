#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

#define INFIN 1e30
#define PI 3.141592653589

typedef struct Interval {
    float min;
    float max;
} Interval;

inline Interval interval_build(float min, float max) {
    Interval out = {.min = min, .max = max};
    return out;
}

inline Interval interval_empty() {
    Interval out = {.min = INFIN, .max = -INFIN};
    return out;
}

inline Interval interval_max() {
    Interval out = {.min = -INFIN, .max = INFIN};
    return out;
}

inline bool interval_contains(Interval *interval, float value) {
    return interval->min <= value && value <= interval->max;
}

inline float interval_clamp(Interval *interval, float value) {
    if (value < interval->min) return interval->min;
    if (value > interval->max) return interval->max;
    return value;
}

inline void color_write(Vec3 pixel_color, FILE *file) {
    Interval interval = interval_build(0.000, 0.999);
    
    int red = (int)(256 * interval_clamp(&interval, pixel_color.x));
    int green = (int)(256 * interval_clamp(&interval, pixel_color.y));
    int blue = (int)(256 * interval_clamp(&interval, pixel_color.z));
    
    fprintf(file, "%d %d %d ", red, green, blue);
}

inline float random_float() {
    return rand() / (RAND_MAX + 1.);
}

inline float random_range(float min, float max) {
    return min + (max - min) * random_float();
}

#endif
