#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

const float INF = std::numeric_limits<float>::infinity();
const float PI = 3.141592653589;

inline float deg_to_rad(float degrees) { return degrees * PI / 180; }

#include "color.h"
#include "ray.h"
#include "vec3.h"

#endif
