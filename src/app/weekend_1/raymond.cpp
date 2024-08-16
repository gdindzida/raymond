#include <iostream>

#include "src/lib/geometry/my_math.hpp"
#include "src/lib/geometry/ray.hpp"
#include "src/lib/types/color.hpp"
#include "src/lib/types/the_types.hpp"

types::floating_point_type hit_sphere(const types::point3& center, double radius,
                                      const geometry::Ray& r) {
    types::vec3 oc = center - r.origin();
    auto a = r.direction().length2();
    auto h = dot(r.direction(), oc);
    auto c = oc.length2() - radius * radius;
    auto discriminant = h * h - a * c;

    if (discriminant < my_math::F_ZERO) {
        return my_math::F_NEG_ONE;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }
}

types::color ray_color(const geometry::Ray& r) {
    auto sphere_center = types::point3(my_math::F_ZERO, my_math::F_ZERO, my_math::F_NEG_ONE);
    auto speher_radius = my_math::F_HALF;
    auto t = hit_sphere(sphere_center, speher_radius, r);

    if (t > my_math::F_ZERO) {
        types::vec3 N = (r.at(t) - sphere_center) / speher_radius;
        return my_math::F_HALF *
               types::color(N.x() + my_math::F_ONE, N.y() + my_math::F_ONE, N.z() + my_math::F_ONE);
    }

    types::vec3 unit_direction = geometry::unit_vector(r.direction());
    auto a = my_math::F_HALF * (unit_direction.y() + my_math::F_ONE);
    const auto blue_color = types::color(0.5, 0.7, 1.0);
    return (my_math::F_ONE - a) * types::C_WHITE + a * blue_color;
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = my_math::F_ONE;
    auto viewport_height = my_math::F_TWO;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = types::point3(my_math::F_ZERO, my_math::F_ZERO, my_math::F_ZERO);

    // Viewport edges vectors
    auto viewport_u = types::vec3(viewport_width, my_math::F_ZERO, my_math::F_ZERO);
    auto viewport_v = types::vec3(my_math::F_ZERO, -viewport_height, my_math::F_ZERO);

    // Viewport delta vectors
    auto pixel_delta_u = viewport_u / double(image_width);
    auto pixel_delta_v = viewport_v / double(image_height);

    // Location of upper left pixel
    auto viewport_upper_left = camera_center +
                               types::vec3(my_math::F_ZERO, my_math::F_ZERO, -focal_length) -
                               my_math::F_HALF * (viewport_u + viewport_v);
    auto pixel00_loc = viewport_upper_left + my_math::F_HALF * (pixel_delta_u + pixel_delta_v);

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
            types::write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.               \n";

    return 0;
}