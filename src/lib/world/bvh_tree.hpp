#ifndef BVH_TREE_HPP
#define BVH_TREE_HPP

#include <array>
#include <memory>

#include "src/lib/world/hittable.hpp"
#include "src/lib/world/sphere.hpp"

namespace world {

class BVHNode : public Hittable {
   public:
    BVHNode() = default;
    BVHNode(const BVHNode& other) = delete;
    BVHNode(BVHNode&& other) = delete;
    ~BVHNode() = default;

    bool hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const override;

    const AABB& bounding_box() const noexcept override;

    static void build_tree(Sphere* const spheres, const size_t& number_of_objects,
                           BVHNode* const nodes, const size_t& number_of_nodes);

   private:
    Hittable* m_left;
    Hittable* m_right;
    AABB m_bbox;

    struct BVHBuildTask {
        size_t start_index;
        size_t end_index;
        size_t node_index;
    };

    static bool box_compare(const Hittable& a, const Hittable& b, int32_t axis_index);

    static bool box_x_compare(const Hittable& a, const Hittable& b);

    static bool box_y_compare(const Hittable& a, const Hittable& b);

    static bool box_z_compare(const Hittable& a, const Hittable& b);
};

}  // namespace world

#endif  // BVH_TREE_HPP
