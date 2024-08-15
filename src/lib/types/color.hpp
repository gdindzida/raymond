#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "src/lib/geometry/vec3.hpp"

namespace types {

template <typename T>
void write_color(std::ostream& out, const Vec3<T>& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

}  // namespace types

#endif  // COLOR_HPP