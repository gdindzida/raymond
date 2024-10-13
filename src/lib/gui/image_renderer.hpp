#ifndef IMAGE_RENDERER_HPP
#define IMAGE_RENDERER_HPP

#include "src/lib/gui/renderer.hpp"

namespace gui {

class ImageRenderer : public Renderer {
   public:
    int samples_per_pixel = 10;

    ImageRenderer() = default;
    ImageRenderer(const ImageRenderer& other) = delete;
    ImageRenderer(ImageRenderer&& other) noexcept = delete;
    ~ImageRenderer() = default;

    void render(const image::Camera& camera, const world::Hittable* world) override;
};

}  // namespace gui

#endif  // IMAGE_RENDERER_HPP
