#ifndef MY_MATH_HPP
#define MY_MATH_HPP

#include "src/lib/types/the_types.hpp"

namespace my_math {

constexpr types::floating_point_type F_ZERO = 0.0;
constexpr types::floating_point_type F_ONE = 1.0;
constexpr types::floating_point_type F_NEG_ONE = -1.0;
constexpr types::floating_point_type F_TWO = 2.0;
constexpr types::floating_point_type F_HALF = 0.5;
constexpr types::floating_point_type F_INF = std::numeric_limits<double>::infinity();
constexpr types::floating_point_type F_PI = 3.1415926535897932385;
constexpr types::floating_point_type F_180 = 180.0;

inline types::floating_point_type degrees_to_radians(types::floating_point_type degrees) {
    return degrees * F_PI / F_180;
}

}  // namespace my_math

#endif  // MY_MATH_HPP