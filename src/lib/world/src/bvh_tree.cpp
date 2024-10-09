#include "src/lib/world/bvh_tree.hpp"

#include <cassert>
#include <queue>

namespace world {

bool BVHNode::hit(const image::Ray& r, const geometry::Interval& t, HitRecord& record) const {
    if (!bbox_.hit(r, t)) return false;

    bool hit_left = left_->hit(r, t, record);
    bool hit_right =
        right_->hit(r, geometry::Interval(t.min(), hit_left ? record.t : t.max()), record);

    return hit_left || hit_right;
}

const AABB& BVHNode::bounding_box() const noexcept { return bbox_; }

void BVHNode::build_tree(Sphere* const spheres, const size_t& number_of_objects,
                         BVHNode* const nodes, const size_t& number_of_nodes) {
#ifndef NDEBUG
    size_t N = pow(2, ceil(log2(number_of_nodes))) - 1;
    assert(N <= number_of_nodes);
#endif

    std::queue<BVHBuildTask> task_queue{};

    task_queue.push(BVHBuildTask{0, number_of_objects, 0});

    while (!task_queue.empty()) {
        BVHBuildTask current_task = task_queue.front();
        task_queue.pop();
        auto start = current_task.start_index;
        auto end = current_task.end_index;
        auto node_index = current_task.node_index;

        nodes[node_index].bbox_ = spheres[start].bounding_box();
        for (size_t object_index = start; object_index < current_task.end_index; ++object_index) {
            nodes[node_index].bbox_ =
                AABB(nodes[node_index].bbox_, spheres[object_index].bounding_box());
        }

        int32_t axis = nodes[node_index].bbox_.longest_axis();

        auto comparator = box_x_compare;
        if (axis == 1) {
            comparator = box_y_compare;
        } else if (axis == 2) {
            comparator = box_z_compare;
        }

        size_t object_span = current_task.end_index - start;
        if (object_span == 1) {
            nodes[node_index].left_ = static_cast<Hittable*>(&spheres[start]);
            nodes[node_index].right_ = static_cast<Hittable*>(&spheres[start]);
        } else if (object_span == 2) {
            nodes[node_index].left_ = static_cast<Hittable*>(&spheres[start]);
            nodes[node_index].right_ = static_cast<Hittable*>(&spheres[start + 1]);
        } else {
            std::sort(spheres + start, spheres + end, comparator);

            auto mid = start + object_span / 2;

            auto left_node_index = 2 * node_index + 1;
            auto right_node_index = 2 * node_index + 2;

            assert(left_node_index < number_of_nodes);
            assert(right_node_index < number_of_nodes);

            task_queue.push(BVHBuildTask{start, mid, left_node_index});
            task_queue.push(BVHBuildTask{mid, end, right_node_index});

            nodes[node_index].left_ = static_cast<Hittable*>(&nodes[left_node_index]);
            nodes[node_index].right_ = static_cast<Hittable*>(&nodes[right_node_index]);
        }
    }
}

bool BVHNode::box_compare(const Hittable& a, const Hittable& b, int32_t axis_index) {
    auto a_axis_interval = a.bounding_box()[axis_index];
    auto b_axis_interval = a.bounding_box()[axis_index];
    return a_axis_interval.min() < b_axis_interval.min();
}

bool BVHNode::box_x_compare(const Hittable& a, const Hittable& b) { return box_compare(a, b, 0); }

bool BVHNode::box_y_compare(const Hittable& a, const Hittable& b) { return box_compare(a, b, 1); }

bool BVHNode::box_z_compare(const Hittable& a, const Hittable& b) { return box_compare(a, b, 2); }

}  // namespace world
