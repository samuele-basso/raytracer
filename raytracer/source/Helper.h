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

}