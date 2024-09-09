#ifndef BVH_NODE_HPP
#define BVH_NODE_HPP

#include "src/lib/world/aabb.hpp"
#include "src/lib/world/hittable.hpp"
#include "src/lib/world/hittable_list.hpp"

namespace world {
class BVHNode : public Hittable {
   public:
    BVHNode() = delete;
    BVHNode(HittableList& list) noexcept;
    BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, const size_t start, const size_t end,
            const uint32_t depth);
    BVHNode(const BVHNode& other) = delete;
    BVHNode(BVHNode&& other) = delete;
    ~BVHNode() = default;

    bool hit(const image::Ray& r, geometry::Interval t, HitRecord& record) const override;

    const AABB& bounding_box() const noexcept override;

    std::shared_ptr<Hittable> left() const noexcept;
    std::shared_ptr<Hittable> right() const noexcept;

   private:
    std::shared_ptr<Hittable> left_;
    std::shared_ptr<Hittable> right_;
    AABB bbox_;
    AABB bbox_temp_;
    uint32_t depth_;

    static bool box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b,
                            int32_t axis_index);

    static bool box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

    static bool box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);

    static bool box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b);
};
}  // namespace world

#endif  // BVH_NODE_HPP