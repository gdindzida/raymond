#include "src/lib/gui/window_renderer.hpp"

#include <chrono>

#include "src/lib/image/color.hpp"
#include "src/lib/multithreading/openmp.hpp"

namespace gui {

WindowRenderer::WindowRenderer(const int width, const int height, const std::string window_name)
    : m_width(width), m_height(height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::clog << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    m_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, m_width, m_height, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::clog << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        std::clog << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                  m_width, m_height);
    if (m_texture == nullptr) {
        std::clog << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
}

WindowRenderer::~WindowRenderer() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
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
            SDL_LockTexture(m_texture, nullptr, &texturePixels, &pitch);

            Uint32* dstPixels = (Uint32*)texturePixels;
            auto start = std::chrono::high_resolution_clock::now();

            OMP_PARALLEL_FOR_DYNAMIC
            for (int y = 0; y < m_height; ++y) {
                for (int x = 0; x < m_width; ++x) {
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

            SDL_UnlockTexture(m_texture);

            SDL_RenderClear(m_renderer);

            SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);

            SDL_RenderPresent(m_renderer);
        }
    }
}

}  // namespace gui
