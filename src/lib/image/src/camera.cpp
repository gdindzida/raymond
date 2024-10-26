#include "src/lib/image/camera.hpp"

#include "src/lib/multithreading/openmp.hpp"

namespace image {

void Camera::initialize() {
    // Image
    m_image_height = int(image_width / aspect_ratio);
    m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    m_pixel_samples_scale = F_ONE / samples_per_pixel;

    // Camera
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / F_TWO);
    auto viewport_height = F_TWO * h * focus_dist;
    auto viewport_width = viewport_height * (double(image_width) / m_image_height);
    m_camera_center = lookFrom;

    // Calculate u, v, w
    vec3_sub(lookFrom, lookAt, m_w);
    vec3_unit(m_w, m_w);
    vec3_cross_product(vup, m_w, m_u);
    vec3_unit(m_u, m_u);
    vec3_cross_product(m_w, m_u, m_v);

    // Viewport edges vectors
    vec3 viewport_u{};
    vec3_scalar_mul(viewport_width, m_u, viewport_u);
    vec3 viewport_v{};
    vec3_scalar_mul(F_NEG_ONE * viewport_height, m_v, viewport_v);

    // Viewport delta vectors
    vec3_scalar_mul(F_ONE / double(image_width), viewport_u, m_pixel_delta_u);
    vec3_scalar_mul(F_ONE / double(m_image_height), viewport_v, m_pixel_delta_v);

    // Location of upper left pixel
    vec3 viewport_u_delta{};
    vec3_scalar_mul(F_NEG_ONE * F_HALF, viewport_u, viewport_u_delta);

    vec3 viewport_v_delta{};
    vec3_scalar_mul(F_NEG_ONE * F_HALF, viewport_v, viewport_v_delta);

    vec3 viewport_w_delta{};
    vec3_scalar_mul(F_NEG_ONE * focus_dist, m_w, viewport_w_delta);

    vec3 viewport_upper_left = m_camera_center;
    viewport_upper_left += viewport_u_delta;
    viewport_upper_left += viewport_v_delta;
    viewport_upper_left += viewport_w_delta;

    vec3_add(m_pixel_delta_u, m_pixel_delta_v, m_pixel00_loc);
    m_pixel00_loc *= F_HALF;
    m_pixel00_loc += viewport_upper_left;

    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / F_TWO));
    vec3_scalar_mul(defocus_radius, m_u, m_defocus_disk_u);
    vec3_scalar_mul(defocus_radius, m_v, m_defocus_disk_v);
}

image::color Camera::pixel_color(uint32_t u, uint32_t v, const world::Hittable* world) const {
    world::HitRecord record{};  // TODO: optimize by allocating only one record per ray??
    Ray r = get_ray(u, v);      // MEM: Needs 3 x vec3
    ray_color(r, max_depth, *world, record);  // MEM: Needs 3 x vec3 + record
    return record.result_color;
}

int Camera::get_image_width() const { return image_width; }
int Camera::get_image_height() const { return m_image_height; }

Ray Camera::get_ray(int i, int j) const {
    auto offset = sample_square();
    vec3 delta_u{};  // TODO: optimize memory
    vec3_scalar_mul(i + offset.x(), m_pixel_delta_u, delta_u);
    vec3 delta_v{};  // TODO: ooptimize memory
    vec3_scalar_mul(j + offset.y(), m_pixel_delta_v, delta_v);
    auto pixel_sample = m_pixel00_loc;  // TODO: optimize memory
    pixel_sample += delta_u;
    pixel_sample += delta_v;

    auto ray_origin = (defocus_angle <= 0) ? m_camera_center : defocus_disk_sample();
    vec3 ray_direction{};  // TODO: optimize memory
    vec3_sub(pixel_sample, ray_origin, ray_direction);
    auto ray_time = random_number();

    return Ray(ray_origin, ray_direction, ray_time);
}

Camera::vec3 Camera::sample_square() const {
    return vec3(random_number() - F_HALF, random_number() - F_HALF, F_ZERO);
}

Camera::point3 Camera::defocus_disk_sample() const {
    auto p = geometry::random_vec3_in_unit_disk();
    vec3 delta_u{};  // TODO: optimize memory
    vec3_scalar_mul(p[0], m_defocus_disk_u, delta_u);
    vec3 delta_v{};  // TODO: ooptimize memory
    vec3_scalar_mul(p[1], m_defocus_disk_v, delta_v);
    auto result = m_camera_center;  // TODO: optimize memory
    result += delta_u;
    result += delta_v;

    return result;
}

void Camera::ray_color(const Ray& r, int depth, const world::Hittable& world,
                       world::HitRecord& ray_record) const {
    if (depth <= 0) {
        ray_record.result_color = C_BLACK;
        return;
    }

    if (world.hit(r, geometry::Interval(F_EPS, F_INF), ray_record)) {
        Ray scattered;
        color attenuation;
        if (ray_record.material->scatter(r, ray_record, attenuation, scattered)) {
            ray_color(scattered, depth - 1, world, ray_record);
            vec3_elementwise_mul(attenuation, ray_record.result_color, ray_record.result_color);
            return;
        }
        ray_record.result_color = C_BLACK;
        return;
    }

    vec3 unit_direction{};  // TODO: optimize memory
    vec3_unit(r.direction(), unit_direction);
    auto a = F_HALF * (unit_direction.y() + F_ONE);

    vec3_scalar_mul(F_ONE - a, C_WHITE, ray_record.result_color);
    color temp{};  // TODO: optimize memory
    vec3_scalar_mul(a, C_BLUE_SKY, temp);
    ray_record.result_color += temp;
}

}  // namespace image