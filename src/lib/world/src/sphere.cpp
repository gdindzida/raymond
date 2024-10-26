#include "src/lib/world/sphere.hpp"

namespace world {

Sphere::Sphere(const geometry::point3& center, fp radius, std::shared_ptr<Material> material)
    : m_center(center, geometry::vec3(F_ZERO, F_ZERO, F_ZERO)),
      m_radius(std::fmax(F_ZERO, radius)),
      m_material(material) {
    auto rvec = geometry::vec3(radius, radius, radius);
    geometry::vec3 lower_corner{};
    geometry::vec3 upper_corner{};
    vec3_sub(center, rvec, lower_corner);
    vec3_add(center, rvec, upper_corner);
    m_bbox = AABB(lower_corner, upper_corner);
}

Sphere::Sphere(const geometry::point3& centerStart, const geometry::point3& centerEnd, fp radius,
               std::shared_ptr<Material> material) {
    geometry::vec3 direction{};
    vec3_sub(centerEnd, centerStart, direction);  // TODO: fix naming
    m_center = image::Ray(centerStart, direction);
    m_radius = std::fmax(F_ZERO, radius);
    m_material = material;

    auto rvec = geometry::vec3(radius, radius, radius);
    geometry::vec3 lower_corner{};
    geometry::vec3 upper_corner{};

    vec3_sub(m_center.at(F_ZERO), rvec, lower_corner);
    vec3_add(m_center.at(F_ZERO), rvec, upper_corner);
    AABB box1(lower_corner, upper_corner);  // TODO: fix box naming

    vec3_sub(m_center.at(F_ONE), rvec, lower_corner);
    vec3_add(m_center.at(F_ONE), rvec, upper_corner);
    AABB box2(lower_corner, upper_corner);

    m_bbox = AABB(box1, box2);
}

bool Sphere::hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const {
    geometry::point3 current_center{};  // TODO: optimize memory with record.
    current_center = m_center.at(r.time());
    geometry::vec3 oc{};  // TODO: optimize memory with record.
    vec3_sub(current_center, r.origin(), oc);
    auto a = r.direction().length2();
    auto h = vec3_dot(r.direction(), oc);
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

    geometry::vec3 outward_normal{};  // TODO: optimize memory with record.
    vec3_sub(record.p, current_center, outward_normal);
    outward_normal *= (1 / m_radius);
    record.set_face_normal(r, outward_normal);
    record.material = m_material;

    return true;
};

const AABB& Sphere::bounding_box() const noexcept { return m_bbox; }

}  // namespace world