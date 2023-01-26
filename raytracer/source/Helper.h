#pragma once

namespace Helper
{

    template <typename T>
    void PPMWrite(const T* data, size_t w, size_t h)
    {
        std::cout << "P3\n" << w << ' ' << h << "\n255\n";

        for (size_t y = h - 1; y >= 0; y--)
        {
            for (size_t x = 0; x < w; x++)
            {
                std::cout <<
                    static_cast<int>(255.999 * data[y * w + x]) << ' ' <<
                    static_cast<int>(255.999 * data[y * w + x + 1]) << ' ' <<
                    static_cast<int>(255.999 * data[y * w + x + 2]) << '\n';
            }
        }
    }

    inline double RandomDouble() {
        return rand() / (RAND_MAX + 1.0);
    }

    inline double RandomDouble(double min, double max) {
        return min + (max - min) * RandomDouble();
    }

    inline double Clamp(double x, double min, double max) {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    namespace Math
    {
        const double PI = 3.1415926535897932385;
        const double INF = std::numeric_limits<double>::infinity();

        inline double DegToRad(double degrees) {
            return degrees * PI / 180.0;
        }
    }

}