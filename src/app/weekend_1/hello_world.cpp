#include <iostream>

#include "src/lib/types/color.hpp"
#include "src/lib/types/the_types.hpp"

int main() {
    int image_width = 256;
    int image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto c =
                types::color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.0);

            types::write_color(std::cout, c);
        }
    }

    std::clog << "\rDone.               \n";

    return 0;
}