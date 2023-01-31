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

Vec3 Refract(const Vec3& uv, const Vec3& n, double etaiOverEtat) {
    auto cosTheta = fmin(Dot(Vec3(0, 0, 0) - uv, n), 1.0);
    Vec3 perp = (n * cosTheta + uv) * etaiOverEtat;
    Vec3 para = n * -sqrt(fabs(1.0 - perp.LengthSquared()));
    return perp + para;
}

class Dielectric : public Material {
public:
    Dielectric(double ior) : IOR(ior) { }

    virtual bool Scatter(
        const Ray& r, const HitRecord& hr, Vec3& attenuation, Ray& scattered
    ) const override {
        attenuation = Vec3(1, 1, 1);
        double refractionRatio = hr.FrontFace ? 1.0/IOR : IOR;

        Vec3 unitDirection = UnitVector(r.Direction);
        double cosTheta = fmin(Dot(Vec3(0, 0, 0) - unitDirection, hr.Normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = refractionRatio * sinTheta > 1.0;
        Vec3 direction;

        if (cannotRefract || Reflectance(cosTheta, refractionRatio) > Helper::RandomDouble())
            direction = Reflect(unitDirection, hr.Normal);
        else
            direction = Refract(unitDirection, hr.Normal, refractionRatio);

        scattered = Ray(hr.Pt, direction);
        return true;
    }

    double IOR;


    static double Reflectance(double cosine, double ref_idx) {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};