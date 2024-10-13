#include "src/lib/math/interval.hpp"

namespace geometry {

// Constants

const Interval Interval::empty = Interval(F_INF, -F_INF);
const Interval Interval::universe = Interval(-F_INF, F_INF);

// Functions

Interval::Interval() : m_min(F_INF), m_max(-F_INF) {}

Interval::Interval(fp min, fp max) : m_min(min), m_max(max) {}

Interval::Interval(const Interval& a, const Interval& b) {
    m_min = a.min() <= b.min() ? a.min() : b.min();
    m_max = a.max() >= b.max() ? a.max() : b.max();
}

fp Interval::size() const noexcept { return std::abs(m_max - m_min); }

bool Interval::contains(fp& value) const noexcept { return m_min <= value && value <= m_max; }

bool Interval::surrounds(fp& value) const noexcept { return m_min < value && value < m_max; }

const fp& Interval::max() const noexcept { return m_max; }

const fp& Interval::min() const noexcept { return m_min; }

fp Interval::clamp(fp value) const noexcept {
    if (value < m_min) return m_min;
    if (value > m_max) return m_max;
    return value;
}

Interval Interval::expand(fp delta) const noexcept {
    auto padding = delta / F_TWO;
    return Interval(m_min - padding, m_max + padding);
}

void Interval::set_max(const fp& new_max) noexcept { m_max = new_max; }
void Interval::set_min(const fp& new_min) noexcept { m_min = new_min; }

}  // namespace geometry
