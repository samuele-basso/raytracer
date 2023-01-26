#include <iostream>

#include "Vec3.h"
#include "Ray.h"
#include "Helper.h"
#include "Camera.h"

#include "world/Hittable.h"
#include "world/Sphere.h"

double HitSphereV1(const Vec3& center, double radius, const Ray& r) {
    Vec3 oc = r.Origin - center;
    auto a = Dot(r.Direction, r.Direction);
    auto b = 2.0 * Dot(oc, r.Direction);
    auto c = Dot(oc, oc) - radius * radius;
    auto d = b * b - 4 * a * c;

    if (d < 0)
        return -1.0f;
    else
        return (-b - sqrt(d)) / (2.0 * a);
}

double HitSphereV2(const Vec3& center, double radius, const Ray& r) {
    Vec3 oc = r.Origin - center;
    auto a = r.Direction.LengthSquared();
    auto halfB = Dot(oc, r.Direction);
    auto c = oc.LengthSquared() - radius * radius;
    auto d = halfB * halfB - a * c;
    if (d < 0)
        return -1.0f;
    else
        return (-halfB - sqrt(d)) / a;
}

Vec3 RayColor(const Ray& r, const HittableList& world) {
    HitRecord hr;
    if (world.Hit(r, 0, Helper::Math::INF, hr)) {
        return (hr.Normal + Vec3(1, 1, 1)) * 0.5;
    }
    Vec3 unitDirection = UnitVector(r.Direction);
    auto t = 0.5 * (unitDirection.y + 1.0);
    return Vec3(1.0, 1.0, 1.0)*(1.0 - t) + Vec3(0.5, 0.7, 1.0)*t;
}

int main() {
    const auto aspectRatio = 16.0 / 9.0;
    const int iw = 400;
    const int ih = static_cast<int>(iw / aspectRatio);
    const int samples = 256;

    HittableList world;
    world.Add(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.Add(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

    Camera camera;

    std::cout << "P3\n" << iw << ' ' << ih << "\n255\n";

    for (int j = ih - 1; j >= 0; j--)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < iw; i++)
        {
            Vec3 pixelColor(0, 0, 0);
            for (int s = 0; s < samples; s++)
            {
                auto u = (i + Helper::RandomDouble()) / (iw - 1);
                auto v = (j + Helper::RandomDouble()) / (ih - 1);
                Ray r = camera.GetRay(u, v);
                pixelColor += RayColor(r, world);
            }
            WriteColor(std::cout, pixelColor, samples);
        }
    }
    std::cerr << "\nDone.\n";
}