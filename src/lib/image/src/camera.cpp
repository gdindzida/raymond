#include "src/lib/image/camera.hpp"

namespace image {

void Camera::render(const world::Hittable& world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height_ << "\n255\n";

    for (int j = 0; j < image_height_; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height_ - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(F_ZERO, F_ZERO, F_ZERO);
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                Ray r = get_ray(i, j);
                pixel_color += ray_color(r, max_depth, world);
            }
            write_color(std::cout, pixel_samples_scale_ * pixel_color);
        }
    }

    std::clog << "\rDone.               \n";
}

void Camera::initialize() {
    // Image
    image_height_ = int(image_width / aspect_ratio);
    image_height_ = (image_height_ < 1) ? 1 : image_height_;

    pixel_samples_scale_ = 1.0 / samples_per_pixel;

    // Camera
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / F_TWO);
    auto viewport_height = F_TWO * h * focus_dist;
    auto viewport_width = viewport_height * (double(image_width) / image_height_);
    camera_center_ = lookFrom;

    // Calculate u, v, w
    w_ = geometry::unit_vector(lookFrom - lookAt);
    u_ = geometry::unit_vector(geometry::cross(vup, w_));
    v_ = geometry::cross(w_, u_);

    // Viewport edges vectors
    auto viewport_u = viewport_width * u_;
    auto viewport_v = viewport_height * (-v_);

    // Viewport delta vectors
    pixel_delta_u_ = viewport_u / double(image_width);
    pixel_delta_v_ = viewport_v / double(image_height_);

    // Location of upper left pixel
    auto viewport_upper_left =
        camera_center_ - (focus_dist * w_) - (viewport_u / F_TWO) - (viewport_v / F_TWO);
    pixel00_loc_ = viewport_upper_left + F_HALF * (pixel_delta_u_ + pixel_delta_v_);

    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / F_TWO));
    defocus_disk_u_ = u_ * defocus_radius;
    defocus_disk_v_ = v_ * defocus_radius;
}

Ray Camera::get_ray(int i, int j) const {
    auto offset = sample_square();
    auto pixel_sample =
        pixel00_loc_ + ((i + offset.x()) * pixel_delta_u_) + ((j + offset.y()) * pixel_delta_v_);

    auto ray_origin = (defocus_angle <= 0) ? camera_center_ : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

Camera::vec3 Camera::sample_square() const {
    return vec3(random_number() - F_HALF, random_number() - F_HALF, F_ZERO);
}

Camera::point3 Camera::defocus_disk_sample() const {
    auto p = geometry::random_vec3_in_unit_disk();
    return camera_center_ + (p[0] * defocus_disk_u_) + (p[1] * defocus_disk_v_);
}

color Camera::ray_color(const Ray& r, int depth, const world::Hittable& world) {
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