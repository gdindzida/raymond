#include "src/lib/image/color.hpp"

namespace image {

// TODO: optimize, review
inline fp linear_to_gamma(fp linear_component) {
    if (linear_component > F_ZERO) return std::sqrt(linear_component);

    return 0;
}

color8_t get_scaled_color(color pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const geometry::Interval intesity(F_ZERO, F_ONE - F_EPS);

    uint8_t rbyte = uint8_t(256 * intesity.clamp(r));
    uint8_t gbyte = uint8_t(256 * intesity.clamp(g));
    uint8_t bbyte = uint8_t(256 * intesity.clamp(b));

    return color8_t(rbyte, gbyte, bbyte);
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto scaled_pixel_color = get_scaled_color(pixel_color);

    out << scaled_pixel_color.x() << ' ' << scaled_pixel_color.y() << ' ' << scaled_pixel_color.z()
        << '\n';
}

}  // namespace image