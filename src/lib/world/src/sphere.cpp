#include "src/lib/world/sphere.hpp"

namespace world {

Sphere::Sphere(const geometry::point3& center, fp radius, std::shared_ptr<Material> material)
    : center_(center), radius_(std::fmax(F_ZERO, radius)), material_(material) {}

bool Sphere::hit(const image::Ray& r, geometry::Interval t, HitRecord& record) const {
    geometry::vec3 oc = center_ - r.origin();
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
    record.material = material_;

    return true;
};

}  // namespace world