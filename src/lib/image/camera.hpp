#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "src/lib/image/color.hpp"
#include "src/lib/image/ray.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/hittable.hpp"

namespace image {

class Camera {
   public:
    using vec3 = geometry::vec3;
    using point3 = geometry::point3;

    fp aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    Camera() = default;
    Camera(const Camera& other) = default;
    Camera(Camera&& other) noexcept = default;
    ~Camera() = default;

    void render(const world::Hittable& world);

   private:
    int image_height_;
    fp pixel_samples_scale_;
    point3 camera_center_;
    point3 pixel00_loc_;
    vec3 pixel_delta_u_;
    vec3 pixel_delta_v_;

    void initialize();

    Ray get_ray(int i, int j) const;

    vec3 sample_square() const;

    color ray_color(const Ray& r, int depth, const world::Hittable& world);
};

}  // namespace image

#endif  // CAMERA_HPP