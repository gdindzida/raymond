#ifndef COLOR_HPP
#define COLOR_HPP

#include <iostream>

#include "src/lib/math/common.hpp"
#include "src/lib/math/interval.hpp"
#include "src/lib/math/my_math.hpp"

namespace image {

using color = geometry::vec3;
using color8_t = geometry::Vec3<uint8_t>;

const color C_WHITE = color(F_ONE, F_ONE, F_ONE);
const color C_BLACK = color(F_ZERO, F_ZERO, F_ZERO);
const color C_RED = color(F_ONE, F_ZERO, F_ZERO);
const color C_GREEN = color(F_ZERO, F_ONE, F_ZERO);
const color C_BLUE = color(F_ZERO, F_ZERO, F_ONE);
const color C_BLUE_SKY = color(F_HALF, 7 * F_ONE / 10, F_ONE);

inline fp linear_to_gamma(fp linear_component);

color8_t get_scaled_color(color pixel_color);

void write_color(std::ostream& out, const color& pixel_color);

}  // namespace image

#endif  // COLOR_HPP