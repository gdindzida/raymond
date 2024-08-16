#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "src/lib/geometry/my_math.hpp"
#include "src/lib/types/the_types.hpp"

namespace geometry {

class Interval {
   public:
    Interval() : min_(my_math::F_INF), max_(-my_math::F_INF) {}

    Interval(types::floating_point_type min, types::floating_point_type max)
        : min_(min), max_(max) {}

    types::floating_point_type size() const { return max_ - min_; }

    bool contains(types::floating_point_type &value) const {
        return min_ <= value && value <= max_;
    }

    bool surrounds(types::floating_point_type &value) const { return min_ < value && value < max_; }

    static const Interval empty, universe;

    types::floating_point_type max() { return max_; }
    types::floating_point_type min() { return min_; }

   private:
    types::floating_point_type min_;
    types::floating_point_type max_;
};

const Interval Interval::empty = Interval(my_math::F_INF, -my_math::F_INF);
const Interval Interval::universe = Interval(-my_math::F_INF, my_math::F_INF);

}  // namespace geometry

#endif