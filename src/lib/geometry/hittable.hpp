#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "src/lib/geometry/interval.hpp"
#include "src/lib/geometry/my_math.hpp"
#include "src/lib/geometry/ray.hpp"
#include "src/lib/types/the_types.hpp"

namespace geometry {
struct HitRecord {
    types::point3 p;
    types::vec3 normal;
    types::floating_point_type t;
    bool front_face;

    void set_face_normal(const geometry::Ray& r, const types::vec3& outward_normal) {
        front_face = geometry::dot(r.direction(), outward_normal) < my_math::F_ZERO;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
   public:
    virtual ~Hittable() = default;

    virtual bool hit(const geometry::Ray& r, Interval t, HitRecord& record) const = 0;
};
}  // namespace geometry

#endif  // HITTABLE_HPP