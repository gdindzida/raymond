#include <iostream>

#include "src/lib/image/color.hpp"
#include "src/lib/image/ray.hpp"
#include "src/lib/math/my_math.hpp"

using namespace my_math;
using vec3 = geometry::vec3;
using point3 = geometry::point3;

my_math::fp hit_sphere(const point3& center, double radius, const image::Ray& r) {
    vec3 oc = center - r.origin();
    auto a = r.direction().length2();
    auto h = geometry::dot(r.direction(), oc);
    auto c = oc.length2() - radius * radius;
    auto discriminant = h * h - a * c;

    if (discriminant < F_ZERO) {
        return F_NEG_ONE;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }
}

image::color ray_color(const image::Ray& r) {
    auto sphere_center = point3(F_ZERO, F_ZERO, F_NEG_ONE);
    auto speher_radius = F_HALF;
    auto t = hit_sphere(sphere_center, speher_radius, r);

    if (t > F_ZERO) {
        vec3 N = (r.at(t) - sphere_center) / speher_radius;
        return F_HALF * image::color(N.x() + F_ONE, N.y() + F_ONE, N.z() + F_ONE);
    }

    vec3 unit_direction = geometry::unit_vector(r.direction());
    auto a = F_HALF * (unit_direction.y() + F_ONE);
    const auto blue_color = image::color(0.5, 0.7, 1.0);
    return (F_ONE - a) * image::C_WHITE + a * blue_color;
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
    auto camera_center = point3(F_ZERO, F_ZERO, F_ZERO);

    // Viewport edges vectors
    auto viewport_u = vec3(viewport_width, F_ZERO, F_ZERO);
    auto viewport_v = vec3(F_ZERO, -viewport_height, F_ZERO);

    // Viewport delta vectors
    auto pixel_delta_u = viewport_u / double(image_width);
    auto pixel_delta_v = viewport_v / double(image_height);

    // Location of upper left pixel
    auto viewport_upper_left =
        camera_center + vec3(F_ZERO, F_ZERO, -focal_length) - F_HALF * (viewport_u + viewport_v);
    auto pixel00_loc = viewport_upper_left + F_HALF * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center =
                pixel00_loc + (double(i) * pixel_delta_u) + (double(j) * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            image::Ray r(camera_center, ray_direction);

            image::color pixel_color = ray_color(r);
            image::write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.               \n";

    return 0;
}