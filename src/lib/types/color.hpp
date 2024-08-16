#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "src/lib/geometry/my_math.hpp"
#include "src/lib/geometry/vec3.hpp"
#include "src/lib/types/the_types.hpp"

namespace types {

const color C_WHITE = color(my_math::F_ONE, my_math::F_ONE, my_math::F_ONE);
const color C_BLACK = color(my_math::F_ZERO, my_math::F_ZERO, my_math::F_ZERO);
const color C_RED = color(my_math::F_ONE, my_math::F_ZERO, my_math::F_ZERO);
const color C_GREEN = color(my_math::F_ZERO, my_math::F_ONE, my_math::F_ZERO);
const color C_BLUE = color(my_math::F_ZERO, my_math::F_ZERO, my_math::F_ONE);
const color C_BLUE_SKY = color(my_math::F_HALF, 7 * my_math::F_ONE / 10, my_math::F_ONE);

template <typename T>
void write_color(std::ostream& out, const geometry::Vec3<T>& pixel_color) {
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