#pragma once
#include "Vec3.h"

class Ray
{
public:
    Ray() = default;
    Ray(const Vec3& origin, const Vec3& direction)
        : Origin(origin), Direction(direction) { }

    Vec3 At(double t) const {
        return Origin + (Direction *t);
    }

    Vec3 Origin;
    Vec3 Direction;
};