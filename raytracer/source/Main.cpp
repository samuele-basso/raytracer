#include <iostream>

#include "Vec3.h"
#include "Ray.h"
#include "Helper.h"
#include "Camera.h"

#include "Material.h"
#include "Hittable.h"

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

Vec3 RayColor(const Ray& r, const HittableList& world, int depth) {
    HitRecord hr;

    if (depth <= 0)
        return Vec3(0, 0, 0);

    if (world.Hit(r, 0.001, Helper::Math::INF, hr)) {
        Ray scattered;
        Vec3 attenuation;
        if (hr.Material->Scatter(r, hr, attenuation, scattered))
            return attenuation * RayColor(scattered, world, depth - 1);
        return Vec3(0, 0, 0);
    }

    Vec3 unitDirection = UnitVector(r.Direction);
    auto t = 0.5 * (unitDirection.y + 1.0);
    return Vec3(1.0, 1.0, 1.0)*(1.0 - t) + Vec3(0.5, 0.7, 1.0)*t;
}

int main() {
    const auto aspectRatio = 16.0 / 9.0;
    const int iw = 400;
    const int ih = static_cast<int>(iw / aspectRatio);
    const int samples = 2048;

    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Vec3(0.7, 0.3, 0.3));
    auto material_left = std::make_shared<Metal>(Vec3(0.8, 0.8, 0.8), 0.5);
    auto material_right = std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.2);

    world.Add(std::make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -1.0), 0.5, material_center));
    world.Add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.Add(std::make_shared<Sphere>(Vec3(1.0, 0.0, -1.0), 0.5, material_right));

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
                pixelColor += RayColor(r, world, 100);
            }
            WriteColor(std::cout, pixelColor, samples);
        }
    }
    std::cerr << "\nDone.\n";
}