#include <iostream>

#include "Vec3.h"
#include "Ray.h"
#include "Helper.h"

double HitSphereV1(const Vec3& center, double radius, const Ray& r) {
    Vec3 oc = r.origin - center;
    auto a = Dot(r.direction, r.direction);
    auto b = 2.0 * Dot(oc, r.direction);
    auto c = Dot(oc, oc) - radius * radius;
    auto d = b * b - 4 * a * c;

    if (d < 0)
        return -1.0f;
    else
        return (-b - sqrt(d)) / (2.0 * a);
}

double HitSphereV2(const Vec3& center, double radius, const Ray& r) {
    Vec3 oc = r.origin - center;
    auto a = r.direction.LengthSquared();
    auto halfB = Dot(oc, r.direction);
    auto c = oc.LengthSquared() - radius * radius;
    auto d = halfB * halfB - a * c;
    if (d < 0)
        return -1.0f;
    else
        return (-halfB - sqrt(d)) / a;
}

Vec3 RayColor(Ray& r) {
    double hit = HitSphereV1(Vec3(0, 0, -1), 0.5, r);
    if (hit > 0.0) {
        Vec3 normal = UnitVector(r.At(hit) - Vec3(0, 0, -1));
        return (normal + Vec3(1.0, 1.0, 1.0)) * 0.5;
    }
    Vec3 unitDirection = UnitVector(r.direction);
    auto t = 0.5 * (unitDirection.y + 1.0);
    return Vec3(1.0, 1.0, 1.0)*(1.0 - t) + Vec3(0.5, 0.7, 1.0)*t;
}

int main() {
    const auto aspectRatio = 16.0 / 9.0;
    const int iw = 400;
    const int ih = static_cast<int>(iw / aspectRatio);

    auto vph = 2.0;
    auto vpw = aspectRatio * vph;
    auto focalLength = 1.0;

    auto origin = Vec3(0, 0, 0);
    auto horizontal = Vec3(vpw, 0, 0);
    auto vertical = Vec3(0, vph, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);

    std::cout << "P3\n" << iw << ' ' << ih << "\n255\n";

    for (int j = ih - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < iw; i++)
        {
            auto u = double(i) / (iw - 1);
            auto v = double(j) / (ih - 1);
            Ray r(origin, lowerLeftCorner + horizontal*u + vertical*v - origin);
            Vec3 pixelColor = RayColor(r);
            WriteColor(std::cout, pixelColor);
        }
    }
    std::cerr << "\nDone.\n";
}