#ifndef RAY_HPP
#define RAY_HPP

#include "src/lib/types/the_types.hpp"

namespace geometry {

class Ray {
   public:
    Ray() {}

    Ray(const types::point3& origin, const types::vec3& direction) : o_(origin), dir_(direction) {}

    const types::point3& origin() const { return o_; }
    const types::vec3& direction() const { return dir_; }

    types::point3 at(types::floating_point_type t) const { return o_ + t * dir_; }

   private:
    types::point3 o_;
    types::vec3 dir_;
};

}  // namespace geometry

#endif  // RAY_HPP