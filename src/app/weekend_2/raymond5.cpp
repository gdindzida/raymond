#include <chrono>
#include <iostream>
#include <memory>
#include <queue>

#include "src/lib/image/camera.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/bvh_node.hpp"
#include "src/lib/world/hittable_list.hpp"
#include "src/lib/world/material.hpp"
#include "src/lib/world/sphere.hpp"
#include "src/lib/world/texture.hpp"

using namespace my_math;
using vec3 = geometry::vec3;
using point3 = geometry::point3;

void bouncing_spheres() {
    std::srand(std::time(nullptr));
    // World
    world::HittableList world{};

    auto checker_texture = std ::make_shared<world::CheckerTexture>(
        0.32, image::color(0.2, 0.3, 0.1), image::color(0.9, 0.9, 0.9));
    auto material_ground = std::make_shared<world::Lambertian>(checker_texture);
    world.add(std::make_shared<world::Sphere>(point3(0, -1000, 0), 1000.0, material_ground));

    for (int32_t a = -3; a < 3; ++a) {
        for (int32_t b = -3; b < 3; ++b) {
            auto choose_material = random_number();
            point3 center(a + 0.9 * random_number(), 0.2, b + 0.9 * random_number());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<world::Material> sphere_material;

                if (choose_material < 0.8) {
                    // diffuse
                    auto albedo = geometry::random_vec3() * geometry::random_vec3();
                    sphere_material = std::make_shared<world::Lambertian>(albedo);
                    auto center2 = center + vec3(0, random_number(0, 0.5), 0);
                    world.add(
                        std::make_shared<world::Sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_material < 0.95) {
                    // metal
                    auto albedo = geometry::random_vec3(0.5, 1);
                    auto fuzz = random_number(0, 0.5);
                    sphere_material = std::make_shared<world::Metal>(albedo, fuzz);
                    world.add(std::make_shared<world::Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<world::Dielectric>(1.5);
                    world.add(std::make_shared<world::Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material_1 = std::make_shared<world::Dielectric>(1.5);
    auto material_2 = std::make_shared<world::Lambertian>(image::color(0.4, 0.2, 0.1));
    auto material_3 = std::make_shared<world::Metal>(image::color(0.7, 0.6, 0.5), 0.0);

    world.add(std::make_shared<world::Sphere>(point3(0, 1, 0), 1.0, material_1));
    world.add(std::make_shared<world::Sphere>(point3(-4, 1, 0), 1.0, material_2));
    world.add(std::make_shared<world::Sphere>(point3(4, 1, 0), 1.0, material_3));

    auto node = std::make_shared<world::BVHNode>(world);
    world = world::HittableList(node);

    // Camera
    image::Camera cam{};
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    // Render
    auto start = std::chrono::high_resolution_clock::now();
    cam.render(world);
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Output the time taken in milliseconds
    std::clog << "Time taken: " << duration.count() << " milliseconds" << std::endl;
}

void checkered_spheres() {
    std::srand(std::time(nullptr));
    // World
    world::HittableList world{};

    auto checker_texture = std ::make_shared<world::CheckerTexture>(
        0.32, image::color(0.2, 0.3, 0.1), image::color(0.9, 0.9, 0.9));

    world.add(std::make_shared<world::Sphere>(
        point3(0, -10, 0), 10, std::make_shared<world::Lambertian>(checker_texture)));
    world.add(std::make_shared<world::Sphere>(
        point3(0, 10, 0), 10, std::make_shared<world::Lambertian>(checker_texture)));

    image::Camera cam{};
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    // Render
    auto start = std::chrono::high_resolution_clock::now();
    cam.render(world);
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Output the time taken in milliseconds
    std::clog << "Time taken: " << duration.count() << " milliseconds" << std::endl;
}

int main() {
    switch (2) {
        case 1:
            bouncing_spheres();
            break;
        case 2:
            checkered_spheres();
            break;
    }
    return 0;
}
