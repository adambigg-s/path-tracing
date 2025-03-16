#ifndef INTERVAL_H
#define INTERVAL_H

#include "constants.h"

class Interval {
  public:
    float min;
    float max;

    Interval() : min(INF), max(-INF) {}
    Interval(float min, float max) : min(min), max(max) {}

    float size() const { return max - min; }

    bool contains(float val) const { return min <= val && val <= max; }

    bool surrounds(float val) const { return min < val && val < max; }

    static const Interval Empty, Universe;
};

#endif
