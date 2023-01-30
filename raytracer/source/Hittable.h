#pragma once

#include "Ray.h"
#include "Vec3.h"

#include <memory>
#include <vector>

class Material;

class HitRecord {
public:
    Vec3 Pt;
    Vec3 Normal;
    std::shared_ptr<Material> Material;
    double T;
    bool FrontFace;

    inline void SetNormal(const Ray& r, const Vec3& outwardNormal) {
        FrontFace = Dot(r.Direction, outwardNormal) < 0;
        Normal = FrontFace ? outwardNormal : Vec3(0.0, 0.0, 0.0) - outwardNormal;
    }
};

class Hittable {
public:
    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& hr) const = 0;
};

class HittableList : public Hittable {
public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object) { Add(object); }

    void Clear() { Objects.clear(); }
    void Add(std::shared_ptr<Hittable> object) { Objects.push_back(object); }

    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& hr) const override {
        HitRecord hrTemp;
        bool hitAnything = false;
        auto closest = tMax;

        for (const auto& object : Objects) {
            if (object->Hit(r, tMin, closest, hrTemp)) {
                hitAnything = true;
                closest = hrTemp.T;
                hr = hrTemp;
            }
        }

        return hitAnything;
    }

    std::vector<std::shared_ptr<Hittable>> Objects;
};

class Sphere : public Hittable {
public:
    Sphere() = default;
    Sphere(Vec3 center, double r, std::shared_ptr<Material> mat) : Center(center), Radius(r), Material(mat) { };

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
        hr.Material = Material;

        return true;
    }

    Vec3 Center;
    double Radius;
    std::shared_ptr<Material> Material;
};