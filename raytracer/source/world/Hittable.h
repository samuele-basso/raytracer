#pragma once
#include "Vec3.h"
#include "Ray.h"

struct HitRecord {
    Vec3 Pt;
    Vec3 Normal;
    double Distance;
};

class Hittable {
public:
    virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& hr) const = 0;
};