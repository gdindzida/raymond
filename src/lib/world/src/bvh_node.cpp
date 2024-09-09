#include "src/lib/world/bvh_node.hpp"

#include <ctime>

namespace world {

BVHNode::BVHNode(HittableList& list) noexcept : BVHNode(list.objects, 0, list.objects.size(), 0) {}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, const size_t start,
                 const size_t end, const uint32_t depth) {
    if (end - start <= 0) return;
    depth_ = depth;
    bbox_ = objects[start]->bounding_box();
    for (size_t object_index = start + 1; object_index < end; ++object_index) {
        bbox_ = AABB(bbox_, objects[object_index]->bounding_box());
    }

    int32_t axis = bbox_.longest_axis();

    // Should this be a factory method?
    auto comparator = box_x_compare;
    if (axis == 1) {
        comparator = box_y_compare;
    } else if (axis == 2) {
        comparator = box_z_compare;
    }

    size_t object_span = end - start;
    if (object_span == 1) {
        left_ = right_ = objects[start];
    } else if (object_span == 2) {
        left_ = objects[start];
        right_ = objects[start + 1];
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left_ = std::make_shared<BVHNode>(objects, start, mid, depth + 1);
        right_ = std::make_shared<BVHNode>(objects, mid, end, depth + 1);
    }
}

bool BVHNode::hit(const image::Ray& r, geometry::Interval t, HitRecord& record) const {
    if (!bbox_.hit(r, t)) return false;

    bool hit_left = left_->hit(r, t, record);
    bool hit_right =
        right_->hit(r, geometry::Interval(t.min(), hit_left ? record.t : t.max()), record);

    return hit_left || hit_right;
}

const AABB& BVHNode::bounding_box() const noexcept { return bbox_; }

std::shared_ptr<Hittable> BVHNode::left() const noexcept { return left_; }
std::shared_ptr<Hittable> BVHNode::right() const noexcept { return right_; }

bool BVHNode::box_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b,
                          int32_t axis_index) {
    auto a_axis_interval = a->bounding_box()[axis_index];
    auto b_axis_interval = a->bounding_box()[axis_index];
    return a_axis_interval.min() < b_axis_interval.min();
}

bool BVHNode::box_x_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
    return box_compare(a, b, 0);
}

bool BVHNode::box_y_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
    return box_compare(a, b, 1);
}

bool BVHNode::box_z_compare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
    return box_compare(a, b, 2);
}

}  // namespace world
