#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>

#include "src/lib/image/color.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"

namespace world {
class Texture {
   public:
    virtual ~Texture() = default;

    virtual image::color value(const fp u, const fp v, const geometry::point3& p) const = 0;
};

class SolidColor : public Texture {
   public:
    SolidColor() = delete;
    SolidColor(const image::color& albedo);
    SolidColor(const fp red, const fp green, const fp blue);
    SolidColor(const SolidColor& other) = default;
    SolidColor(SolidColor&& other) noexcept = default;
    ~SolidColor() = default;

    image::color value(const fp u, const fp v, const geometry::point3& p) const override;

   private:
    image::color m_albedo;
};

class CheckerTexture : public Texture {
   public:
    CheckerTexture() = delete;
    CheckerTexture(const fp scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd);
    CheckerTexture(const fp scale, const image::color& c1, const image::color& c2);
    CheckerTexture(const CheckerTexture& other) = default;
    CheckerTexture(CheckerTexture&& other) noexcept = default;
    ~CheckerTexture() = default;

    image::color value(const fp u, const fp v, const geometry::point3& p) const override;

   private:
    fp m_inv_scale;
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;
};

}  // namespace world

#endif  // TEXTURE_HPP