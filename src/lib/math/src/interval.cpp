#include "src/lib/math/interval.hpp"

namespace geometry {

// Constants

const Interval Interval::empty = Interval(F_INF, -F_INF);
const Interval Interval::universe = Interval(-F_INF, F_INF);

// Functions

Interval::Interval() : min_(F_INF), max_(-F_INF) {}

Interval::Interval(fp min, fp max) : min_(min), max_(max) {}

fp Interval::size() const noexcept { return max_ - min_; }

bool Interval::contains(fp& value) const noexcept { return min_ <= value && value <= max_; }

bool Interval::surrounds(fp& value) const noexcept { return min_ < value && value < max_; }

const fp& Interval::max() const noexcept { return max_; }

const fp& Interval::min() const noexcept { return min_; }

fp Interval::clamp(fp value) const noexcept {
    if (value < min_) return min_;
    if (value > max_) return max_;
    return value;
}

}  // namespace geometry
