#include "src/lib/image/ray.hpp"

namespace image {

Ray::Ray(const point3& origin, const vec3& direction) : o_(origin), dir_(direction) {}

Ray& Ray::operator=(const Ray& other) {
    o_ = other.origin();
    dir_ = other.direction();

    return *this;
}

const Ray::point3& Ray::origin() const noexcept { return o_; }

const Ray::vec3& Ray::direction() const noexcept { return dir_; }

Ray::point3 Ray::at(fp t) const noexcept { return o_ + t * dir_; }

}  // namespace image