#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

inline void write_color(std::ostream &out, const Vec3 &pixel_color) {
    int rbyte = int(255.999 * pixel_color.x);
    int gbyte = int(255.999 * pixel_color.y);
    int bbyte = int(255.999 * pixel_color.z);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
