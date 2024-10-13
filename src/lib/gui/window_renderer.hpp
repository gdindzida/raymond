#ifndef WINDOW_RENDERER_HPP
#define WINDOW_RENDERER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

#include "src/lib/gui/renderer.hpp"

namespace gui {

class WindowRenderer : public Renderer {
   public:
    int samples_per_pixel = 10;
    uint8_t alpha = 255ul;

    WindowRenderer() = delete;
    explicit WindowRenderer(const int width, const int height,
                            const std::string window_name = "Raytracer");
    WindowRenderer(const WindowRenderer& other) = delete;
    WindowRenderer(WindowRenderer&& other) noexcept = delete;
    ~WindowRenderer();

    void render(const image::Camera& camera, const world::Hittable* world) override;

   private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    int width_, height_;
};

}  // namespace gui

#endif  // WINDOW_RENDERER_HPP
