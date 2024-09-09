#include "src/lib/math/interval.hpp"

namespace geometry {

// Constants

const Interval Interval::empty = Interval(F_INF, -F_INF);
const Interval Interval::universe = Interval(-F_INF, F_INF);

// Functions

Interval::Interval() : min_(F_INF), max_(-F_INF) {}

Interval::Interval(fp min, fp max) : min_(min), max_(max) {}

Interval::Interval(const Interval& a, const Interval& b) {
    min_ = a.min() <= b.min() ? a.min() : b.min();
    max_ = a.max() >= b.max() ? a.max() : b.max();
}

fp Interval::size() const noexcept { return std::abs(max_ - min_); }

bool Interval::contains(fp& value) const noexcept { return min_ <= value && value <= max_; }

bool Interval::surrounds(fp& value) const noexcept { return min_ < value && value < max_; }

const fp& Interval::max() const noexcept { return max_; }

const fp& Interval::min() const noexcept { return min_; }

fp Interval::clamp(fp value) const noexcept {
    if (value < min_) return min_;
    if (value > max_) return max_;
    return value;
}

Interval Interval::expand(fp delta) const noexcept {
    auto padding = delta / F_TWO;
    return Interval(min_ - padding, max_ + padding);
}

void Interval::set_max(const fp& new_max) noexcept { max_ = new_max; }
void Interval::set_min(const fp& new_min) noexcept { min_ = new_min; }

}  // namespace geometry
