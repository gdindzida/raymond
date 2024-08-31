#include "src/lib/world/hittable_list.hpp"

namespace world {

HittableList::HittableList(std::shared_ptr<Hittable> object) { add(object); }

void HittableList::clear() { objects.clear(); }

void HittableList::add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

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

}  // namespace world
