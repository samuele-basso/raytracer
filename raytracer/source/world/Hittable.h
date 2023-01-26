#pragma once
#include "Vec3.h"
#include "Ray.h"

#include <memory>
#include <vector>

class HitRecord {
public:
    Vec3 Pt;
    Vec3 Normal;
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