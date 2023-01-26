#pragma once
#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere() = default;
    Sphere(Vec3 center, double r) : Center(center), Radius(r) { };

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& hr) const override {
        Vec3 oc = r.origin - Center;
        auto a = r.direction.LengthSquared();
        auto halfB = Dot(oc, r.direction);
        auto c = oc.LengthSquared() - Radius * Radius;
        auto d = halfB * halfB - a * c;
        if (d < 0) return false;
        auto sqrtd = sqrt(d);

        auto root = (-halfB - sqrtd) / a;
        if (root < tMin || root > tMax) {
            root = (-halfB + sqrtd) / a;
            if (root < tMin || tMax < root)
                return false;
        }

        hr.Distance = root;
        hr.Pt = r.At(hr.Distance);
        hr.Normal = (hr.Pt - Center) / Radius;

        return true;
    }

    Vec3 Center;
    float Radius;
};