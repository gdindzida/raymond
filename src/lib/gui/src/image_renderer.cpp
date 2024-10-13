#include "src/lib/gui/image_renderer.hpp"

#include "src/lib/multithreading/openmp.hpp"

namespace gui {

void ImageRenderer::render(const image::Camera& camera, const world::Hittable* world) {
    auto image_width = camera.get_image_width();
    auto image_height = camera.get_image_height();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            image::color pixel_color(F_ZERO, F_ZERO, F_ZERO);
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                pixel_color += camera.pixel_color(i, j, world);
            }
            image::write_color(std::cout, (F_ONE / samples_per_pixel) * pixel_color);
        }
    }

    std::clog << "\rDone.               \n";
}

}  // namespace gui