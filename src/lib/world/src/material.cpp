#include "src/lib/world/material.hpp"

namespace world {

Lambertian::Lambertian(const color& albedo) : albedo_(albedo) {}

bool Lambertian::scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                         Ray& scattered) const {
    auto scatter_direction = record.normal + geometry::random_unit_vec3();

    if (scatter_direction.near_zero()) scatter_direction = record.normal;

    scattered = Ray(record.p, scatter_direction);
    attenuation = albedo_;
    return true;
}

Metal::Metal(const color& albedo, fp fuzz) : albedo_(albedo), fuzz_(fuzz < F_ONE ? fuzz : F_ONE) {}

bool Metal::scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                    Ray& scattered) const {
    geometry::vec3 reflected = geometry::reflect(r_in.direction(), record.normal);
    reflected = geometry::unit_vector(reflected) + (fuzz_ * geometry::random_unit_vec3());
    scattered = Ray(record.p, reflected);
    attenuation = albedo_;
    return geometry::dot(scattered.direction(), record.normal) > 0;
}

Dielectric::Dielectric(fp refraction_index) : eta_(refraction_index) {};

bool Dielectric::scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                         Ray& scattered) const {
    attenuation = image::C_WHITE;
    fp ri = record.front_face ? (F_ONE / eta_) : eta_;

    geometry::vec3 unit_direction = geometry::unit_vector(r_in.direction());
    fp cos_theta = std::fmin(dot(-unit_direction, record.normal), F_ONE);
    fp sin_theta = std::sqrt(F_ONE - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > F_ONE;
    geometry::vec3 direction;
    if (cannot_refract || (reflectance(cos_theta, ri) > random_number())) {
        direction = geometry::reflect(unit_direction, record.normal);
    } else {
        direction = geometry::refract(unit_direction, record.normal, ri);
    }

    scattered = image::Ray(record.p, direction);
    return true;
}

fp Dielectric::reflectance(fp cosine, fp refraction_index) const {
    auto r0 = (F_ONE - refraction_index) / (F_ONE + refraction_index);
    r0 = r0 * r0;
    return r0 + (F_ONE - r0) * std::pow((F_ONE - cosine), 5);
}

}  // namespace world
