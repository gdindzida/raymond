#include "src/lib/image/color.hpp"

namespace image {

// TODO: optimize, review
inline fp linear_to_gamma(fp linear_component) {
    if (linear_component > F_ZERO) return std::sqrt(linear_component);

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const geometry::Interval intesity(F_ZERO, F_ONE - F_EPS);

    int rbyte = int(256 * intesity.clamp(r));
    int gbyte = int(256 * intesity.clamp(g));
    int bbyte = int(256 * intesity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

}  // namespace image