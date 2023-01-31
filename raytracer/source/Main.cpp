#include <iostream>

#include "Vec3.h"
#include "Ray.h"
#include "Helper.h"
#include "Camera.h"

#include "Material.h"
#include "Hittable.h"

HittableList Scene() {
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = Helper::RandomDouble();
            Vec3 center(a + 0.9 * Helper::RandomDouble(), 0.2, b + 0.9 * Helper::RandomDouble());

            if (sqrt((center - Vec3(4, 0.2, 0)).LengthSquared()) > 0.9) {
                std::shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    auto albedo = Vec3::Random() * Vec3::Random();
                    sphereMaterial = std::make_shared<Lambertian>(albedo);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95) {
                    auto albedo = Vec3::Random(0.5, 1);
                    auto fuzz = Helper::RandomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
                else {
                    sphereMaterial = std::make_shared<Dielectric>(1.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1));
    world.Add(std::make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    return world;
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
    const auto aspectRatio = 3.0 / 2.0;
    const int iw = 1200;
    const int ih = static_cast<int>(iw / aspectRatio);
    const int samples = 512;

    HittableList world = Scene();

    auto ground = std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0));
    auto center = std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5));
    auto left = std::make_shared<Dielectric>(1.5);
    auto right = std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 0.0);

    world.Add(std::make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, ground));
    world.Add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -1.0), 0.5, center));
    world.Add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, left));
    world.Add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), -0.45, left));
    world.Add(std::make_shared<Sphere>(Vec3(1.0, 0.0, -1.0), 0.5, right));

    Vec3 lookfrom(13, 2, 3);
    Vec3 lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto focusDistance = 10.0;
    auto aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20, aspectRatio, aperture, focusDistance);

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
                pixelColor += RayColor(r, world, 50);
            }
            WriteColor(std::cout, pixelColor, samples);
        }
    }
    std::cerr << "\nDone.\n";
}