#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include <memory>

#include "src/lib/image/ray.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/interval.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/aabb.hpp"
#include "src/lib/world/hit_record.hpp"
#include "src/lib/world/material.hpp"

namespace world {

class Material;

class Hittable {
   public:
    virtual ~Hittable() = default;

    virtual bool hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const = 0;

    virtual const AABB& bounding_box() const noexcept = 0;
};

}  // namespace world

#endif  // HITTABLE_HPP