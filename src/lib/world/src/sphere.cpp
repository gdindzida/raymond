#include "src/lib/world/sphere.hpp"

namespace world {

Sphere::Sphere(const geometry::point3& center, fp radius, std::shared_ptr<Material> material)
    : center_(center, geometry::vec3(F_ZERO, F_ZERO, F_ZERO)),
      radius_(std::fmax(F_ZERO, radius)),
      material_(material) {
    auto rvec = geometry::vec3(radius, radius, radius);
    bbox_ = AABB(center - rvec, center + rvec);
}

Sphere::Sphere(const geometry::point3& centerStart, const geometry::point3& centerEnd, fp radius,
               std::shared_ptr<Material> material)
    : center_(centerStart, centerEnd - centerStart),
      radius_(std::fmax(F_ZERO, radius)),
      material_(material) {
    auto rvec = geometry::vec3(radius, radius, radius);
    AABB box1(center_.at(F_ZERO) - rvec, center_.at(F_ZERO) + rvec);
    AABB box2(center_.at(F_ONE) - rvec, center_.at(F_ONE) + rvec);
    bbox_ = AABB(box1, box2);
}

bool Sphere::hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const {
    const geometry::point3 current_center = center_.at(r.time());
    geometry::vec3 oc = current_center - r.origin();
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
    auto outward_normal = (record.p - current_center) / radius_;
    record.set_face_normal(r, outward_normal);
    record.material = material_;

    return true;
};

const AABB& Sphere::bounding_box() const noexcept { return bbox_; }

}  // namespace world