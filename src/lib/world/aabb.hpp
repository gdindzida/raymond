#ifndef AABB_HPP
#define AABB_HPP

#include "src/lib/image/ray.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/interval.hpp"

namespace world {
class AABB {
   public:
    using point3 = geometry::point3;
    using vec3 = geometry::vec3;
    using Interval = geometry::Interval;

    AABB() = default;
    AABB(const geometry::point3& a, const geometry::point3& b);
    AABB(const AABB& a, const AABB& b);
    AABB(const Interval& x, const Interval& y, const Interval& z);
    AABB(const AABB& other) noexcept = default;
    AABB(AABB&& other) noexcept = default;
    ~AABB() = default;

    AABB& operator=(const AABB& other) noexcept = default;

    const geometry::Interval& operator[](const int32_t& index) const noexcept;

    bool hit(const image::Ray r, const geometry::Interval& ray_t) const noexcept;

    int32_t longest_axis() const noexcept;

    static const AABB empty, universe;

    std::string to_string() const noexcept;

   private:
    std::array<geometry::Interval, 3> m_intervals;
};

}  // namespace world
#endif  // AABB_HPP
