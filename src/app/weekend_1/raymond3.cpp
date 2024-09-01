#include <iostream>
#include <memory>

#include "src/lib/image/camera.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/hittable_list.hpp"
#include "src/lib/world/material.hpp"
#include "src/lib/world/sphere.hpp"

using namespace my_math;
using vec3 = geometry::vec3;
using point3 = geometry::point3;

int main() {
    // World
    world::HittableList world{};

    auto material_ground = std::make_shared<world::Lambertian>(image::color(0.8, 0.8, 0));
    auto material_center = std::make_shared<world::Lambertian>(image::color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<world::Dielectric>(1.5);
    auto material_bubble = std::make_shared<world::Dielectric>(1.0 / 1.5);
    auto material_right = std::make_shared<world::Metal>(image::color(0.8, 0.6, 0.2), 1.0);

    world.add(
        std::make_shared<world::Sphere>(point3(F_ZERO, F_ZERO, -1.2), F_HALF, material_center));
    world.add(
        std::make_shared<world::Sphere>(point3(F_ZERO, -100.5, F_NEG_ONE), 100.0, material_ground));
    world.add(std::make_shared<world::Sphere>(point3(F_NEG_ONE, F_ZERO, F_NEG_ONE), F_HALF,
                                              material_left));
    world.add(std::make_shared<world::Sphere>(point3(F_NEG_ONE, F_ZERO, F_NEG_ONE), 0.4,
                                              material_bubble));
    world.add(
        std::make_shared<world::Sphere>(point3(F_ONE, F_ZERO, F_NEG_ONE), F_HALF, material_right));

    // Camera
    image::Camera cam{};
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.vfov = 20;
    cam.lookFrom = point3(-2, 2, 1);
    cam.lookAt = point3(0, 0, -1);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 10;
    cam.focus_dist = 3.4;

    // Render
    cam.render(world);

    return 0;
}