#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <memory>

#include "src/lib/image/ray.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/hittable.hpp"
#include "src/lib/world/material.hpp"

namespace world {

class Sphere : public Hittable {
   public:
    Sphere() = default;
    Sphere(const geometry::point3& center, fp radius, std::shared_ptr<Material> material);
    Sphere(const geometry::point3& centerStart, const geometry::point3& centerEnd, fp radius,
           std::shared_ptr<Material> material);
    Sphere(const Sphere& other) = default;
    Sphere(Sphere&& other) noexcept = default;
    ~Sphere() = default;

    Sphere& operator=(const Sphere& other) = default;

    bool hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const override;

    const AABB& bounding_box() const noexcept override;

   private:
    image::Ray m_center;
    fp m_radius;
    std::shared_ptr<Material> m_material;
    AABB m_bbox;
};

}  // namespace world

#endif