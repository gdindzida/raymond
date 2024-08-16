#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <memory>

#include "src/lib/geometry/hittable.hpp"
#include "src/lib/geometry/my_math.hpp"
#include "src/lib/types/the_types.hpp"

namespace geometry {

class Sphere : public Hittable {
   public:
    Sphere(const types::point3& center, types::floating_point_type radius)
        : center_(center), radius_(std::fmax(my_math::F_ZERO, radius)) {}

    bool hit(const geometry::Ray& r, Interval t, HitRecord& record) const override {
        types::vec3 oc = center_ - r.origin();
        auto a = r.direction().length2();
        auto h = dot(r.direction(), oc);
        auto c = oc.length2() - radius_ * radius_;
        auto discriminant = h * h - a * c;

        if (discriminant < my_math::F_ZERO) {
            return false;
        }

        auto sqrtd = std::sqrt(discriminant);

        // Find nearest valid root.
        auto root = (h - sqrtd) / a;
        if (!t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!t.surrounds(root)) return false;
        }

        record.t = root;
        record.p = r.at(root);
        auto outward_normal = (record.p - center_) / radius_;
        record.set_face_normal(r, outward_normal);

        return true;
    };

   private:
    types::point3 center_;
    types::floating_point_type radius_;
};

}  // namespace geometry

#endif