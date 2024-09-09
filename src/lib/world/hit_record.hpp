#ifndef HIT_RECORD_HPP
#define HIT_RECORD_HPP

#include <memory>

#include "src/lib/image/ray.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"

namespace world {

class Material;

struct HitRecord {
    using vec3 = geometry::vec3;
    using point3 = geometry::point3;

    point3 p;
    vec3 normal;
    std::shared_ptr<Material> material;
    fp t;
    fp u;
    fp v;
    bool front_face;

    void set_face_normal(const image::Ray& r, const vec3& outward_normal) {
        front_face = geometry::dot(r.direction(), outward_normal) < F_ZERO;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

}  // namespace world

#endif  // HIT_RECORD_HPP
