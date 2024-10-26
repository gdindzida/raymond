#include "src/lib/image/ray.hpp"

namespace image {

Ray::Ray(const point3& origin, const vec3& direction)
    : m_o(origin), m_dir(direction), m_time(F_ZERO) {}

Ray::Ray(const point3& origin, const vec3& direction, const fp time)
    : m_o(origin), m_dir(direction), m_time(time) {}

Ray& Ray::operator=(const Ray& other) {
    m_o = other.origin();
    m_dir = other.direction();

    return *this;
}

const Ray::point3& Ray::origin() const noexcept { return m_o; }

const Ray::vec3& Ray::direction() const noexcept { return m_dir; }

Ray::point3 Ray::at(fp t) const noexcept {
    vec3 temp;
    geometry::vec3_scalar_mul(t, m_dir, temp);
    temp += m_o;
    return temp;
}

fp Ray::time() const noexcept { return m_time; }

}  // namespace image