#pragma once
#include <iostream>

#include "Helper.h"

class Vec3
{
public:
    Vec3() : x(0), y(0), z(0) { }
    Vec3(double x, double y, double z) : x(x), y(y), z(z) { }

    double operator[](int i) const { return *(&x+i); }

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator*=(const double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double LengthSquared() const {
        return x*x + y*y + z*z;
    }

    bool NearZero() const {
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    inline static Vec3 Random() {
        return Vec3(Helper::RandomDouble(), Helper::RandomDouble(), Helper::RandomDouble());
    }

    inline static Vec3 Random(double min, double max) {
        return Vec3(Helper::RandomDouble(min, max), Helper::RandomDouble(min, max), Helper::RandomDouble(min, max));
    }

public:
    double x, y, z;
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x+v.x, u.y+v.y, u.z+v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x-v.x, u.y-v.y, u.z-v.z);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x*v.x, u.y*v.y, u.z*v.z);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return Vec3(t*v.x, t*v.y, t*v.z);
}

inline Vec3 operator/(Vec3 v, double t) {
    return v * (1/t);
}

inline double Dot(const Vec3& u, const Vec3& v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.y*v.z - u.z*v.y,
                u.z*v.x - u.x*v.z,
                u.x*v.y - u.y*v.x);
}

inline Vec3 UnitVector(Vec3 v) {
    return v / std::sqrt(v.LengthSquared());
}

Vec3 RandomPtUnitSphere() {
    while (true) {
        auto p = Vec3::Random(-1, 1);
        if (p.LengthSquared() < 1) return p;
    }
}

Vec3 RandomUnitVector() {
    return UnitVector(RandomPtUnitSphere());
}

void WriteColor(std::ostream& out, Vec3 pixelColor, int samples) {
    auto r = pixelColor.x;
    auto g = pixelColor.y;
    auto b = pixelColor.z;

    auto scale = 1.0 / samples;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << static_cast<int>(255.999 * Helper::Clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * Helper::Clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * Helper::Clamp(b, 0.0, 0.999)) << '\n';
}