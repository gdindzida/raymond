#include "src/lib/world/texture.hpp"

namespace world {

SolidColor::SolidColor(const image::color& albedo) : albedo_(albedo) {}

SolidColor::SolidColor(const fp red, const fp green, const fp blue)
    : SolidColor(image::color(red, green, blue)) {}

image::color SolidColor::value(const fp u, const fp v, const geometry::point3& p) const {
    return albedo_;
}

CheckerTexture::CheckerTexture(const fp scale, std::shared_ptr<Texture> even,
                               std::shared_ptr<Texture> odd)
    : inv_scale_(F_ONE / scale), even_(even), odd_(odd) {}

CheckerTexture::CheckerTexture(const fp scale, const image::color& c1, const image::color& c2)
    : CheckerTexture(scale, std::make_shared<SolidColor>(c1), std::make_shared<SolidColor>(c2)) {}

image::color CheckerTexture::value(const fp u, const fp v, const geometry::point3& p) const {
    auto xInt = int32_t(std::floor(inv_scale_ * p.x()));
    auto yInt = int32_t(std::floor(inv_scale_ * p.y()));
    auto zInt = int32_t(std::floor(inv_scale_ * p.z()));

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? even_->value(u, v, p) : odd_->value(u, v, p);
}

}  // namespace world
