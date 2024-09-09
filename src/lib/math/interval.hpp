#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "src/lib/math/my_math.hpp"

namespace geometry {

class Interval {
   public:
    Interval();
    Interval(fp min, fp max);
    Interval(const Interval& a, const Interval& b);
    Interval(const Interval& other) = default;
    Interval(Interval&& other) noexcept = default;
    ~Interval() = default;

    Interval& operator=(const Interval& other) noexcept = default;

    fp size() const noexcept;
    bool contains(fp& value) const noexcept;
    bool surrounds(fp& value) const noexcept;
    const fp& max() const noexcept;
    const fp& min() const noexcept;
    fp clamp(fp value) const noexcept;
    Interval expand(fp delta) const noexcept;
    void set_max(const fp& new_max) noexcept;
    void set_min(const fp& new_min) noexcept;

    static const Interval empty, universe;

   private:
    fp min_;
    fp max_;
};

}  // namespace geometry

#endif