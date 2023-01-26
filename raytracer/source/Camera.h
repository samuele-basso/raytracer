#pragma once
#include "Helper.h"
#include "Vec3.h"
#include "Ray.h"

class Camera {
public:
    Camera() {
        auto aspect = 16.0 / 9.0;
        auto viewportHeight = 2.0;
        auto viewportWidth = aspect * viewportHeight;
        auto focalLength = 1.0;

        Origin = Vec3(0, 0, 0);
        Horizontal = Vec3(viewportWidth, 0.0, 0.0);
        Vertical = Vec3(0.0, viewportHeight, 0.0);
        LowerLeftCorner = Origin - Horizontal / 2 - Vertical / 2 - Vec3(0, 0, focalLength);
    }

    Ray GetRay(double u, double v) const {
        return Ray(Origin, LowerLeftCorner + Horizontal*u + Vertical*v - Origin);
    }

private:
    Vec3 Origin;
    Vec3 LowerLeftCorner;
    Vec3 Horizontal, Vertical;
};