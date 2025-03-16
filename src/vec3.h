#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

typedef struct Vec3 {
    float x;
    float y;
    float z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    Vec3 operator+(const Vec3 &vec) const {
        return Vec3(x + vec.x, y + vec.y, z + vec.z);
    }

    Vec3 operator-(const Vec3 &vec) const {
        return Vec3(x - vec.x, y - vec.y, z - vec.z);
    }

    Vec3 operator*(const Vec3 &vec) const {
        return Vec3(x * vec.x, y * vec.y, z * vec.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(const Vec3 &vec) const {
        return Vec3(x / vec.x, y / vec.y, z / vec.z);
    }

    Vec3 operator/(float scalar) const {
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    Vec3 &operator+=(const Vec3 &vec) {
        x += vec.x;
        y += vec.y;
        z += vec.z;
        return *this;
    }

    Vec3 &operator-=(const Vec3 &vec) {
        x -= vec.x;
        y -= vec.y;
        z -= vec.z;
        return *this;
    }

    Vec3 &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3 &operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float inner_product(const Vec3 &vec) const {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    Vec3 cross_product(const Vec3 &vec) const {
        return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z,
                    x * vec.y - y * vec.x);
    }

    float length() const { return sqrt(this->inner_product(*this)); }

    Vec3 normalized() const {
        float len = this->length();
        if (len > 0) {
            float invlen = 1. / len;
            return Vec3(x * invlen, y * invlen, z * invlen);
        }
        return *this;
    }
} Vec3;

#endif
