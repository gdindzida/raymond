#ifndef VEC3_H
#define VEC3_H

#include <array>
#include <cmath>
#include <iostream>

#include "src/lib/math/my_math.hpp"

namespace geometry {

template <typename T>
class Vec3 {
   public:
    Vec3() : e_{} {}
    Vec3(T e0, T e1, T e2) : e_{e0, e1, e2} {}
    Vec3(const Vec3& other) = default;
    Vec3(Vec3&& other) noexcept = default;
    ~Vec3() = default;

    const T& x() const noexcept { return e_[0]; }
    const T& y() const noexcept { return e_[1]; }
    const T& z() const noexcept { return e_[2]; }

    Vec3<T>& operator=(const Vec3<fp>& other) {
        e_[0] = other.x();
        e_[1] = other.y();
        e_[2] = other.z();

        return *this;
    }

    T operator[](int i) const { return e_[i]; }

    T& operator[](int i) { return e_[i]; }

    Vec3<T> operator-() const noexcept { return Vec3<T>(-e_[0], -e_[1], -e_[2]); }  // TODO:optimize

    Vec3<T>& operator+=(const Vec3<T>& v) noexcept {
        e_[0] += v.x();
        e_[1] += v.y();
        e_[2] += v.z();

        return *this;
    }

    Vec3<T>& operator*=(T t) noexcept {
        e_[0] *= t;
        e_[1] *= t;
        e_[2] *= t;

        return *this;
    }

    Vec3<T>& operator/=(T t) noexcept { return *this *= 1 / t; }

    T length() const noexcept { return std::sqrt(length2()); }

    T length2() const noexcept { return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]; }

    bool near_zero() const noexcept {
        return (std::fabs(e_[0]) < F_EPS_2) && (std::fabs(e_[1]) < F_EPS_2) &&
               (std::fabs(e_[2]) < F_EPS_2);
    }

   private:
    std::array<T, 3> e_{};
};

}  // namespace geometry

#endif  // VEC3_H