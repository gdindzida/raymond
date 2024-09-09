#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "src/lib/image/color.hpp"
#include "src/lib/image/ray.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/hit_record.hpp"
#include "src/lib/world/hittable.hpp"
#include "src/lib/world/texture.hpp"

namespace world {

using Ray = image::Ray;
using color = image::color;

class Material {
   public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                         Ray& scattered) const = 0;
};

class Lambertian : public Material {
   public:
    Lambertian() = delete;
    Lambertian(const color& albedo);
    Lambertian(std::shared_ptr<Texture> texture);
    Lambertian(const Lambertian& other) = default;
    Lambertian(Lambertian&& other) noexcept = default;
    ~Lambertian() = default;

    bool scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                 Ray& scattered) const override;

   private:
    std::shared_ptr<Texture> texture_;
};

class Metal : public Material {
   public:
    Metal() = delete;
    Metal(const color& albedo, fp fuzz);
    Metal(const Metal& other) = default;
    Metal(Metal&& other) noexcept = default;
    ~Metal() = default;

    bool scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                 Ray& scattered) const override;

   private:
    color albedo_;
    fp fuzz_;
};

class Dielectric : public Material {
   public:
    Dielectric() = delete;
    Dielectric(fp refraction_index);
    Dielectric(const Dielectric& other) = default;
    Dielectric(Dielectric&& other) noexcept = default;

    bool scatter(const Ray& r_in, const HitRecord& record, color& attenuation,
                 Ray& scattered) const override;

   private:
    fp eta_;

    fp reflectance(fp cosine, fp refraction_index) const;
};

}  // namespace world

#endif  // MATERIAL_HPP