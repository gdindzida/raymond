#ifndef RAY_HPP
#define RAY_HPP

#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"

namespace image {

class Ray {
   public:
    using vec3 = geometry::vec3;
    using point3 = geometry::point3;

    Ray() = default;
    Ray(const point3& origin, const vec3& direction);
    Ray(const point3& origin, const vec3& direction, const fp time);
    Ray(const Ray& other) = default;
    Ray(Ray&& other) noexcept = default;
    ~Ray() = default;

    Ray& operator=(const Ray& other);

    const point3& origin() const noexcept;
    const vec3& direction() const noexcept;
    point3 at(fp t) const noexcept;
    fp time() const noexcept;

   private:
    point3 m_o;
    vec3 m_dir;
    fp m_time;
};

}  // namespace image

#endif  // RAY_HPP