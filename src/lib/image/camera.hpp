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
    fp vfov = 90;
    point3 lookFrom = point3(0, 0, 0);
    point3 lookAt = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);
    fp defocus_angle = 0;
    fp focus_dist = 10;

    Camera() = default;
    Camera(const Camera& other) = default;
    Camera(Camera&& other) noexcept = default;
    ~Camera() = default;

    void initialize();

    image::color pixel_color(uint32_t u, uint32_t v, const world::Hittable* world) const;

    int get_image_width() const;
    int get_image_height() const;

   private:
    int m_image_height;
    fp m_pixel_samples_scale;
    point3 m_camera_center;
    point3 m_pixel00_loc;
    vec3 m_pixel_delta_u;
    vec3 m_pixel_delta_v;
    vec3 m_u, m_v, m_w;
    vec3 m_defocus_disk_u;
    vec3 m_defocus_disk_v;

    Ray get_ray(int i, int j) const;

    vec3 sample_square() const;

    point3 defocus_disk_sample() const;

    void ray_color(const Ray& r, int depth, const world::Hittable& world,
                   world::HitRecord& ray_record) const;
};

}  // namespace image

#endif  // CAMERA_HPP