#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <memory>
#include <vector>

#include "src/lib/image/ray.hpp"
#include "src/lib/math/interval.hpp"
#include "src/lib/world/hittable.hpp"

namespace world {

class HittableList : public Hittable {
   public:
    std::vector<std::shared_ptr<Hittable>> objects{};

    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object);
    HittableList(const HittableList& other) = delete;
    HittableList(HittableList&& other) noexcept = delete;
    ~HittableList() = default;

    void clear();

    void add(std::shared_ptr<Hittable> object);

    bool hit(const image::Ray& r, geometry::Interval t, HitRecord& rec) const override;
};

}  // namespace world

#endif  // HITTABLE_LIST_HPP
