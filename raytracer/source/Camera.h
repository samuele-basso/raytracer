#pragma once
#include "Helper.h"
#include "Vec3.h"
#include "Ray.h"

class Camera {
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, double fov, double aspectRatio, double aperture, double focusDistance) {
        auto theta = Helper::Math::DegToRad(fov);
        auto h = tan(theta / 2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = aspectRatio * viewportHeight;

        auto focalLength = 1.0;
        W = UnitVector(lookFrom - lookAt);
        U = UnitVector(Cross(vup, W));
        V = Cross(W, U);

        Origin = lookFrom;
        Horizontal = U*viewportWidth*focusDistance;
        Vertical = V*viewportHeight*focusDistance;
        LowerLeftCorner = Origin - Horizontal / 2 - Vertical / 2 - W*focusDistance;

        LensRadius = aperture / 2;
    }

    Ray GetRay(double s, double t) const {
        Vec3 rd = RandomUnitDisk()*LensRadius;
        Vec3 offset = U * rd.x + V * rd.y;

        return Ray(
            Origin + offset,
            LowerLeftCorner + Horizontal*s + Vertical*t - Origin - offset
        );

        return Ray(Origin, LowerLeftCorner + Horizontal*s + Vertical*t - Origin);
    }

private:
    Vec3 Origin;
    Vec3 LowerLeftCorner;
    Vec3 Horizontal, Vertical;
    Vec3 U, V, W;
    double LensRadius;
};