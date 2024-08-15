#include <iostream>

#include "src/lib/geometry/ray.hpp"
#include "src/lib/types/color.hpp"
#include "src/lib/types/the_types.hpp"

bool hit_sphere(const types::point3& center, double radius, const geometry::Ray& r) {
    types::vec3 oc = center - r.origin();
    auto a = types::dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return (discriminant >= 0.0);
}

types::color ray_color(const geometry::Ray& r) {
    if (hit_sphere(types::point3(0.0, 0.0, -1.0), 0.5, r)) return types::color(1.0, 0.0, 0.0);

    types::vec3 unit_direction = types::unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * types::color(1.0, 1.0, 1.0) + a * types::color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = types::point3(0, 0, 0);

    // Viewport edges vectors
    auto viewport_u = types::vec3(viewport_width, 0.0, 0.0);
    auto viewport_v = types::vec3(0.0, -viewport_height, 0.0);

    // Viewport delta vectors
    auto pixel_delta_u = viewport_u / double(image_width);
    auto pixel_delta_v = viewport_v / double(image_height);

    // Location of upper left pixel
    auto viewport_upper_left =
        camera_center + types::vec3(0.0, 0.0, -focal_length) - 0.5 * (viewport_u + viewport_v);
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center =
                pixel00_loc + (double(i) * pixel_delta_u) + (double(j) * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            geometry::Ray r(camera_center, ray_direction);

            types::color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.               \n";

    return 0;
}