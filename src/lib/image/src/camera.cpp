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
    m_w = geometry::unit_vector(lookFrom - lookAt);
    m_u = geometry::unit_vector(geometry::cross(vup, m_w));
    m_v = geometry::cross(m_w, m_u);

    // Viewport edges vectors
    auto viewport_u = viewport_width * m_u;
    auto viewport_v = viewport_height * (-m_v);

    // Viewport delta vectors
    m_pixel_delta_u = viewport_u / double(image_width);
    m_pixel_delta_v = viewport_v / double(m_image_height);

    // Location of upper left pixel
    auto viewport_upper_left =
        m_camera_center - (focus_dist * m_w) - (viewport_u / F_TWO) - (viewport_v / F_TWO);
    m_pixel00_loc = viewport_upper_left + F_HALF * (m_pixel_delta_u + m_pixel_delta_v);

    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / F_TWO));
    m_defocus_disk_u = m_u * defocus_radius;
    m_defocus_disk_v = m_v * defocus_radius;
}

void Camera::render(const world::Hittable& world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << m_image_height << "\n255\n";

    OMP_PARALLEL_FOR_DYNAMIC
    for (int j = 0; j < m_image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(F_ZERO, F_ZERO, F_ZERO);
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                Ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            // write_color(std::cout, pixel_samples_scale_ * pixel_color);
        }
    }

    std::clog << "\rDone.               \n";
}

image::color Camera::pixel_color(uint32_t u, uint32_t v, const world::Hittable* world) const {
    Ray r = get_ray(u, v);
    return ray_color(r, max_depth, *world);
}

int Camera::get_image_width() const { return image_width; }
int Camera::get_image_height() const { return m_image_height; }

Ray Camera::get_ray(int i, int j) const {
    auto offset = sample_square();
    auto pixel_sample =
        m_pixel00_loc + ((i + offset.x()) * m_pixel_delta_u) + ((j + offset.y()) * m_pixel_delta_v);

    auto ray_origin = (defocus_angle <= 0) ? m_camera_center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;
    auto ray_time = random_number();

    return Ray(ray_origin, ray_direction, ray_time);
}

Camera::vec3 Camera::sample_square() const {
    return vec3(random_number() - F_HALF, random_number() - F_HALF, F_ZERO);
}

Camera::point3 Camera::defocus_disk_sample() const {
    auto p = geometry::random_vec3_in_unit_disk();
    return m_camera_center + (p[0] * m_defocus_disk_u) + (p[1] * m_defocus_disk_v);
}

color Camera::ray_color(const Ray& r, int depth, const world::Hittable& world) const {
    if (depth <= 0) return C_BLACK;

    world::HitRecord record{};
    if (world.hit(r, geometry::Interval(F_EPS, F_INF), record)) {
        Ray scattered;
        color attenuation;
        if (record.material->scatter(r, record, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth - 1, world);
        }
        return C_BLACK;
    }

    vec3 unit_direction = geometry::unit_vector(r.direction());
    auto a = F_HALF * (unit_direction.y() + F_ONE);
    return (F_ONE - a) * C_WHITE + a * C_BLUE_SKY;
}

}  // namespace image