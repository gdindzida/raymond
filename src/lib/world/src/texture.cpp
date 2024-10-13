#include "src/lib/world/texture.hpp"

namespace world {

SolidColor::SolidColor(const image::color& albedo) : m_albedo(albedo) {}

SolidColor::SolidColor(const fp red, const fp green, const fp blue)
    : SolidColor(image::color(red, green, blue)) {}

image::color SolidColor::value(const fp u, const fp v, const geometry::point3& p) const {
    return m_albedo;
}

CheckerTexture::CheckerTexture(const fp scale, std::shared_ptr<Texture> even,
                               std::shared_ptr<Texture> odd)
    : m_inv_scale(F_ONE / scale), m_even(even), m_odd(odd) {}

CheckerTexture::CheckerTexture(const fp scale, const image::color& c1, const image::color& c2)
    : CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2)) {}

image::color CheckerTexture::value(const fp u, const fp v, const geometry::point3& p) const {
    auto xInt = int32_t(std::floor(m_inv_scale * p.x()));
    auto yInt = int32_t(std::floor(m_inv_scale * p.y()));
    auto zInt = int32_t(std::floor(m_inv_scale * p.z()));

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? m_even->value(u, v, p) : m_odd->value(u, v, p);
}

}  // namespace world
