#include "src/lib/world/sphere.hpp"

namespace world {

Sphere::Sphere(const geometry::point3& center, fp radius, std::shared_ptr<Material> material)
    : m_center(center, geometry::vec3(F_ZERO, F_ZERO, F_ZERO)),
      m_radius(std::fmax(F_ZERO, radius)),
      m_material(material) {
    auto rvec = geometry::vec3(radius, radius, radius);
    m_bbox = AABB(center - rvec, center + rvec);
}

Sphere::Sphere(const geometry::point3& centerStart, const geometry::point3& centerEnd, fp radius,
               std::shared_ptr<Material> material)
    : m_center(centerStart, centerEnd - centerStart),
      m_radius(std::fmax(F_ZERO, radius)),
      m_material(material) {
    auto rvec = geometry::vec3(radius, radius, radius);
    AABB box1(m_center.at(F_ZERO) - rvec, m_center.at(F_ZERO) + rvec);
    AABB box2(m_center.at(F_ONE) - rvec, m_center.at(F_ONE) + rvec);
    m_bbox = AABB(box1, box2);
}

bool Sphere::hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const {
    const geometry::point3 current_center = m_center.at(r.time());
    geometry::vec3 oc = current_center - r.origin();
    auto a = r.direction().length2();
    auto h = dot(r.direction(), oc);
    auto c = oc.length2() - m_radius * m_radius;
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
    auto outward_normal = (record.p - current_center) / m_radius;
    record.set_face_normal(r, outward_normal);
    record.material = m_material;

    return true;
};

const AABB& Sphere::bounding_box() const noexcept { return m_bbox; }

}  // namespace world