#include "src/lib/gui/window_renderer.hpp"

#include <chrono>

#include "src/lib/image/color.hpp"
#include "src/lib/multithreading/openmp.hpp"

namespace gui {

WindowRenderer::WindowRenderer(const int width, const int height, const std::string window_name)
    : width_(width), height_(height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::clog << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    window_ = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width_, height_, SDL_WINDOW_SHOWN);
    if (!window_) {
        std::clog << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == nullptr) {
        std::clog << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                 width_, height_);
    if (texture_ == nullptr) {
        std::clog << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }
}

WindowRenderer::~WindowRenderer() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void WindowRenderer::render(const image::Camera& camera, const world::Hittable* world) {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        bool space_pressed = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                // Check if the pressed key is "Space"
                if (e.key.keysym.sym == SDLK_SPACE) {
                    space_pressed = true;  // Space key pressed, exit the loop
                }
            }
        }

        if (space_pressed) {
            // Lock the texture for direct pixel access
            void* texturePixels;
            int pitch;
            SDL_LockTexture(texture_, nullptr, &texturePixels, &pitch);

            Uint32* dstPixels = (Uint32*)texturePixels;
            auto start = std::chrono::high_resolution_clock::now();

            OMP_PARALLEL_FOR_DYNAMIC
            for (int y = 0; y < height_; ++y) {
                for (int x = 0; x < width_; ++x) {
                    // Raytracer
                    auto color = camera.pixel_color(x, y, world);

                    auto scaled_color = image::get_scaled_color(color);

                    // Pack the color into a Uint32 (ARGB format)
                    dstPixels[y * (pitch / sizeof(Uint32)) + x] =
                        (alpha << 24) | (scaled_color.x() << 16) | (scaled_color.y() << 8) |
                        scaled_color.z();
                }
            }
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::clog << "Time taken: " << duration.count() << " milliseconds" << std::endl;

            SDL_UnlockTexture(texture_);

            SDL_RenderClear(renderer_);

            SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);

            SDL_RenderPresent(renderer_);
        }
    }
}

}  // namespace gui
