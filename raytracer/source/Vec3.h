#pragma once
#include <iostream>

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

void WriteColor(std::ostream& out, Vec3 pixelColor) {
    out << static_cast<int>(255.999 * pixelColor.x) << ' '
        << static_cast<int>(255.999 * pixelColor.y) << ' '
        << static_cast<int>(255.999 * pixelColor.z) << '\n';
}