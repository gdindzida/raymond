#include <chrono>
#include <iostream>
#include <memory>
#include <queue>

#include "src/lib/gui/window_renderer.hpp"
#include "src/lib/image/camera.hpp"
#include "src/lib/math/common.hpp"
#include "src/lib/math/my_math.hpp"
#include "src/lib/world/bvh_tree.hpp"
#include "src/lib/world/material.hpp"
#include "src/lib/world/sphere.hpp"
#include "src/lib/world/texture.hpp"

using namespace my_math;
using vec3 = geometry::vec3;
using point3 = geometry::point3;

constexpr size_t max_number_of_spheres = 50;
constexpr size_t max_number_of_nodes = 127;

void add_sphere(const world::Sphere& sphere,
                std::array<world::Sphere, max_number_of_spheres>& world,
                size_t& number_of_spheres) {
    world[number_of_spheres] = sphere;
    number_of_spheres++;
}

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));

    // World
    std::array<world::Sphere, max_number_of_spheres> world{};
    std::array<world::BVHNode, max_number_of_nodes> nodes{};

    size_t number_of_spheres = 0;

    auto checker_texture = std ::make_shared<world::CheckerTexture>(
        0.32, image::color(0.2, 0.3, 0.1), image::color(0.9, 0.9, 0.9));
    auto material_ground = std::make_shared<world::Lambertian>(checker_texture);
    add_sphere(world::Sphere(point3(0, -1000, 0), 1000.0, material_ground), world,
               number_of_spheres);

    for (int32_t a = -3; a < 3; ++a) {
        for (int32_t b = -3; b < 3; ++b) {
            auto choose_material = random_number();
            point3 center(a + 0.9 * random_number(), 0.2, b + 0.9 * random_number());

            vec3 spawn_vector{};
            vec3_sub(center, point3(4, 0.2, 0), spawn_vector);
            if (spawn_vector.length() > 0.9) {
                std::shared_ptr<world::Material> sphere_material;

                if (choose_material < 0.8) {
                    // diffuse
                    vec3 albedo{};
                    vec3_elementwise_mul(geometry::random_vec3(), geometry::random_vec3(), albedo);

                    sphere_material = std::make_shared<world::Lambertian>(albedo);

                    vec3 center2{};
                    vec3_add(center, vec3(0, random_number(0, 0.5), 0), center2);
                    add_sphere(world::Sphere(center, center2, 0.2, sphere_material), world,
                               number_of_spheres);
                } else if (choose_material < 0.95) {
                    // metal
                    auto albedo = geometry::random_vec3(0.5, 1);
                    auto fuzz = random_number(0, 0.5);
                    sphere_material = std::make_shared<world::Metal>(albedo, fuzz);
                    add_sphere(world::Sphere(center, 0.2, sphere_material), world,
                               number_of_spheres);
                } else {
                    // glass
                    sphere_material = std::make_shared<world::Dielectric>(1.5);
                    add_sphere(world::Sphere(center, 0.2, sphere_material), world,
                               number_of_spheres);
                }
            }
        }
    }

    auto material_1 = std::make_shared<world::Dielectric>(1.5);
    auto material_2 = std::make_shared<world::Lambertian>(image::color(0.4, 0.2, 0.1));
    auto material_3 = std::make_shared<world::Metal>(image::color(0.7, 0.6, 0.5), 0.0);

    add_sphere(world::Sphere(point3(0, 1, 0), 1.0, material_1), world, number_of_spheres);
    add_sphere(world::Sphere(point3(-4, 1, 0), 1.0, material_2), world, number_of_spheres);
    add_sphere(world::Sphere(point3(4, 1, 0), 1.0, material_3), world, number_of_spheres);

    world::BVHNode::build_tree(world.data(), number_of_spheres, nodes.data(), max_number_of_nodes);

    // Camera setup
    image::Camera cam{};
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 15;
    cam.max_depth = 10;
    cam.vfov = 20;
    cam.lookFrom = point3(13, 2, 3);
    cam.lookAt = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);
    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;
    cam.initialize();

    // Render
    gui::WindowRenderer renderer{cam.get_image_width(), cam.get_image_height()};

    renderer.render(cam, &nodes[0]);

    return 0;
}
