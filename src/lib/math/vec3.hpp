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
    Vec3() : m_e{} {}
    Vec3(T e0, T e1, T e2) : m_e{e0, e1, e2} {}
    Vec3(const Vec3& other) = default;
    Vec3(Vec3&& other) noexcept = default;
    ~Vec3() = default;

    const T& x() const noexcept { return m_e[0]; }
    const T& y() const noexcept { return m_e[1]; }
    const T& z() const noexcept { return m_e[2]; }

    Vec3<T>& operator=(const Vec3<fp>& other) {
        m_e[0] = other.x();
        m_e[1] = other.y();
        m_e[2] = other.z();

        return *this;
    }

    T operator[](int i) const { return m_e[i]; }

    T& operator[](int i) { return m_e[i]; }

    Vec3<T> operator-() const noexcept {
        return Vec3<T>(-m_e[0], -m_e[1], -m_e[2]);
    }  // TODO:optimize

    Vec3<T>& operator+=(const Vec3<T>& v) noexcept {
        m_e[0] += v.x();
        m_e[1] += v.y();
        m_e[2] += v.z();

        return *this;
    }

    Vec3<T>& operator*=(T t) noexcept {
        m_e[0] *= t;
        m_e[1] *= t;
        m_e[2] *= t;

        return *this;
    }

    Vec3<T>& operator/=(T t) noexcept { return *this *= 1 / t; }

    T length() const noexcept { return std::sqrt(length2()); }

    T length2() const noexcept { return m_e[0] * m_e[0] + m_e[1] * m_e[1] + m_e[2] * m_e[2]; }

    bool near_zero() const noexcept {
        return (std::fabs(m_e[0]) < F_EPS_2) && (std::fabs(m_e[1]) < F_EPS_2) &&
               (std::fabs(m_e[2]) < F_EPS_2);
    }

   private:
    std::array<T, 3> m_e{};
};

}  // namespace geometry

#endif  // VEC3_H