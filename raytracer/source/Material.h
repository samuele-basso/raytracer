#pragma once
#include "Vec3.h"
#include "Hittable.h"

class HitRecord;

class Material {
public:
    virtual bool Scatter(
        const Ray& r, const HitRecord& hr, Vec3& attenuation, Ray& scattered
    ) const = 0;
};

Vec3 Reflect(const Vec3& v, const Vec3& n) {
    return v - n * 2 * Dot(v, n);
}

class Metal : public Material {
public:
    Metal(const Vec3& albedo, double fuzz) : Albedo(albedo), Fuzz(fuzz) { }

    virtual bool Scatter(
        const Ray& r, const HitRecord& hr, Vec3& attenuation, Ray& scattered
    ) const override {
        auto reflect = Reflect(UnitVector(r.Direction), hr.Normal);
        scattered = Ray(hr.Pt, reflect + RandomPtUnitSphere() * Fuzz);
        attenuation = Albedo;
        return (Dot(scattered.Direction, hr.Normal) > 0);
    }

    Vec3 Albedo;
    double Fuzz;
};

class Lambertian : public Material {
public:
    Lambertian(const Vec3& color) : Albedo(color) { }

    virtual bool Scatter(
        const Ray& r, const HitRecord& hr, Vec3& attenuation, Ray& scattered
    ) const override {
        auto scatterDirection = hr.Normal + RandomUnitVector();
        if (scatterDirection.NearZero())
            scatterDirection = hr.Normal;
        scattered = Ray(hr.Pt, scatterDirection);
        attenuation = Albedo;
        return true;
    }

    Vec3 Albedo;
};