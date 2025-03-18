#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
    if (value < interval->min)
        return interval->min;
    if (value > interval->max)
        return interval->max;
    return value;
}

inline float linear_to_gamma(float value) {
    if (value > 0) {
        return sqrt(value);
    }
    return 0;
}

inline float random_float() { return rand() / (RAND_MAX + 1.); }

inline float random_range(float min, float max) { return min + (max - min) * random_float(); }

#endif
