#include "src/lib/world/aabb.hpp"

namespace world {

const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

AABB::AABB(const point3& a, const point3& b) {
    m_intervals[0] = (a[0] < b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
    m_intervals[1] = (a[1] < b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
    m_intervals[2] = (a[2] < b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
}

AABB::AABB(const AABB& a, const AABB& b) {
    m_intervals[0] = Interval(a[0], b[0]);
    m_intervals[1] = Interval(a[1], b[1]);
    m_intervals[2] = Interval(a[2], b[2]);
}

AABB::AABB(const Interval& x, const Interval& y, const Interval& z) {
    m_intervals[0] = x;
    m_intervals[1] = y;
    m_intervals[2] = z;
}

const geometry::Interval& AABB::operator[](const int32_t& index) const noexcept {
    return m_intervals[index];
}

bool AABB::hit(const image::Ray r, const Interval& ray_t) const noexcept {
    const point3& ray_origin = r.origin();
    const vec3& ray_direction = r.direction();

    Interval temp = ray_t;

    for (int axis = 0; axis < 3; ++axis) {
        const Interval& ax = m_intervals[axis];
        const fp adinv = F_ONE / ray_direction[axis];

        auto t0 = (ax.min() - ray_origin[axis]) * adinv;
        auto t1 = (ax.max() - ray_origin[axis]) * adinv;

        // calculate overlap
        if (t0 < t1) {
            if (t0 > temp.min()) temp.set_min(t0);
            if (t1 < temp.max()) temp.set_max(t1);
        } else {
            if (t1 > temp.min()) temp.set_min(t1);
            if (t0 < temp.max()) temp.set_max(t0);
        }

        if (temp.max() <= temp.min()) return false;
    }
    return true;
}

int32_t AABB::longest_axis() const noexcept {
    if (m_intervals[0].size() > m_intervals[1].size()) {
        return m_intervals[0].size() > m_intervals[2].size() ? 0 : 2;
    } else {
        return m_intervals[1].size() > m_intervals[2].size() ? 1 : 2;
    }
}

std::string AABB::to_string() const noexcept {
    std::string output = "";
    output += "(" + std::to_string(m_intervals[0].min()) + "," +
              std::to_string(m_intervals[0].max()) + " )" + " (" +
              std::to_string(m_intervals[1].min()) + "," + std::to_string(m_intervals[1].max()) +
              " )" + " (" + std::to_string(m_intervals[2].min()) + "," +
              std::to_string(m_intervals[2].max()) + " )";
    return output;
}

}  // namespace world
