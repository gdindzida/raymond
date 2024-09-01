#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <memory>

#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/hittable.hpp"
#include "src/lib/world/material.hpp"

namespace world {

class Sphere : public Hittable {
   public:
    Sphere() = delete;
    Sphere(const geometry::point3& center, fp radius, std::shared_ptr<Material> material);
    Sphere(const Sphere& other) = default;
    Sphere(Sphere&& other) noexcept = default;
    ~Sphere() = default;

    bool hit(const image::Ray& r, geometry::Interval t, HitRecord& record) const override;

   private:
    geometry::point3 center_;
    fp radius_;
    std::shared_ptr<Material> material_;
};

}  // namespace world

#endif