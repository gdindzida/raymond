#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <memory>
#include <vector>

#include "src/lib/geometry/hittable.hpp"
#include "src/lib/geometry/interval.hpp"

namespace geometry {

class HittableList : public Hittable {
   public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {};
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    bool hit(const geometry::Ray& r, Interval t, HitRecord& rec) const override {
        HitRecord temp_record;
        bool hit_anything = false;
        auto closest_so_far = t.max();

        for (const auto& object : objects) {
            if (object->hit(r, Interval(t.min(), closest_so_far), temp_record)) {
                hit_anything = true;
                closest_so_far = temp_record.t;
                rec = temp_record;
            }
        }

        return hit_anything;
    }
};

}  // namespace geometry

#endif  // HITTABLE_LIST_HPP