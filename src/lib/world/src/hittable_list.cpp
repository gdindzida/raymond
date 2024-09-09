#include "src/lib/world/hittable_list.hpp"

namespace world {

HittableList::HittableList(std::shared_ptr<Hittable> object) {
    add(object);
    bbox_ = object->bounding_box();
}

void HittableList::clear() { objects.clear(); }

void HittableList::add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
    bbox_ = AABB(bbox_, object->bounding_box());
}

bool HittableList::hit(const image::Ray& r, geometry::Interval t, HitRecord& rec) const {
    HitRecord temp_record;
    bool hit_anything = false;
    auto closest_so_far = t.max();

    for (const auto& object : objects) {
        if (object->hit(r, geometry::Interval(t.min(), closest_so_far), temp_record)) {
            hit_anything = true;
            closest_so_far = temp_record.t;
            rec = temp_record;
        }
    }

    return hit_anything;
}

const AABB& HittableList::bounding_box() const noexcept { return bbox_; }

}  // namespace world
