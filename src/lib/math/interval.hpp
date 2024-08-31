#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "src/lib/math/my_math.hpp"

namespace geometry {

class Interval {
   public:
    Interval();
    Interval(fp min, fp max);
    Interval(const Interval& other) = default;
    Interval(Interval&& other) noexcept = default;
    ~Interval() = default;

    fp size() const noexcept;
    bool contains(fp& value) const noexcept;
    bool surrounds(fp& value) const noexcept;
    const fp& max() const noexcept;
    const fp& min() const noexcept;
    fp clamp(fp value) const noexcept;

    static const Interval empty, universe;

   private:
    fp min_;
    fp max_;
};

}  // namespace geometry

#endif