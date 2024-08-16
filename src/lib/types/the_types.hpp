#ifndef THE_TYPES_HPP
#define THE_TYPES_HPP

#include "src/lib/geometry/vec3.hpp"

namespace types {

using floating_point_type = double;

using vec3 = geometry::Vec3<floating_point_type>;

using point3 = vec3;

using color = vec3;

}  // namespace types

#endif  // THE_TYPES_HPP