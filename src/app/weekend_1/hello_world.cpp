#include <iostream>

#include "src/lib/image/color.hpp"
#include "src/lib/math/my_math.hpp"

int main() {
    int image_width = 256;
    int image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto c =
                image::color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.0);

            image::write_color(std::cout, c);
        }
    }

    std::clog << "\rDone.               \n";

    return 0;
}