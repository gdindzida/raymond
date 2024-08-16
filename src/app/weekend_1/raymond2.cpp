#include <iostream>
#include <memory>

#include "src/lib/geometry/hittable.hpp"
#include "src/lib/geometry/hittable_list.hpp"
#include "src/lib/geometry/interval.hpp"
#include "src/lib/geometry/my_math.hpp"
#include "src/lib/geometry/ray.hpp"
#include "src/lib/geometry/sphere.hpp"
#include "src/lib/types/color.hpp"
#include "src/lib/types/the_types.hpp"

using namespace my_math;

types::color ray_color(const geometry::Ray& r, const geometry::HittableList& world) {
    geometry::HitRecord record{};
    if (world.hit(r, geometry::Interval(0, F_INF), record)) {
        return F_HALF * (record.normal + types::C_WHITE);
    }

    types::vec3 unit_direction = geometry::unit_vector(r.direction());
    auto a = F_HALF * (unit_direction.y() + F_ONE);
    return (F_ONE - a) * types::C_WHITE + a * types::C_BLUE_SKY;
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = F_ONE;
    auto viewport_height = F_TWO;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = types::point3(F_ZERO, F_ZERO, F_ZERO);

    // World
    geometry::HittableList world{};
    world.add(std::make_shared<geometry::Sphere>(types::point3(F_ZERO, F_ZERO, F_NEG_ONE), F_HALF));
    world.add(std::make_shared<geometry::Sphere>(types::point3(F_ZERO, -100.5, F_NEG_ONE), 100));

    // Viewport edges vectors
    auto viewport_u = types::vec3(viewport_width, F_ZERO, F_ZERO);
    auto viewport_v = types::vec3(F_ZERO, -viewport_height, F_ZERO);

    // Viewport delta vectors
    auto pixel_delta_u = viewport_u / double(image_width);
    auto pixel_delta_v = viewport_v / double(image_height);

    // Location of upper left pixel
    auto viewport_upper_left = camera_center + types::vec3(F_ZERO, F_ZERO, -focal_length) -
                               F_HALF * (viewport_u + viewport_v);
    auto pixel00_loc = viewport_upper_left + F_HALF * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center =
                pixel00_loc + (double(i) * pixel_delta_u) + (double(j) * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            geometry::Ray r(camera_center, ray_direction);

            types::color pixel_color = ray_color(r, world);
            types::write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.               \n";

    return 0;
}