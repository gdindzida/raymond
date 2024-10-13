#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "src/lib/image/camera.hpp"
#include "src/lib/world/hittable.hpp"

namespace gui {

class Renderer {
   public:
    virtual ~Renderer() = default;

    virtual void render(const image::Camera& camera, const world::Hittable* world) = 0;
};

}  // namespace gui

#endif  // RENDERER_HPP