#pragma once
#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere() = default;
    Sphere(Vec3 center, double r) : Center(center), Radius(r) { };

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& hr) const override {
        Vec3 oc = r.Origin - Center;
        auto a = r.Direction.LengthSquared();
        auto halfB = Dot(oc, r.Direction);
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

        hr.T = root;
        hr.Pt = r.At(hr.T);
        hr.SetNormal(r, (hr.Pt - Center) / Radius);

        return true;
    }

    Vec3 Center;
    float Radius;
};