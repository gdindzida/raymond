#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

namespace geometry {

template <typename T>
class Vec3 {
   public:
    Vec3() : e_{} {}
    Vec3(T e0, T e1, T e2) : e_{e0, e1, e2} {}

    T x() const { return e_[0]; }
    T y() const { return e_[1]; }
    T z() const { return e_[2]; }

    Vec3<T> operator-() const { return Vec3<T>(-e_[0], -e_[1], -e_[2]); }
    T operator[](int i) const { return e_[i]; }
    T& operator[](int i) { return e_[i]; }

    Vec3<T>& operator+=(const Vec3<T>& v) {
        e_[0] += v.x();
        e_[1] += v.y();
        e_[2] += v.z();

        return *this;
    }

    Vec3<T>& operator*=(T t) {
        e_[0] *= t;
        e_[1] *= t;
        e_[2] *= t;

        return *this;
    }

    Vec3<T>& operator/=(T t) { return *this *= 1 / t; }

    T length() const { return std::sqrt(length2()); }

    T length2() const { return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]; }

   private:
    T e_[3];
};

// Vector utility functions

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vec3<T>& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

template <typename T>
inline Vec3<T> operator+(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

template <typename T>
inline Vec3<T> operator-(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3<T>(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

template <typename T>
inline Vec3<T> operator*(T t, const Vec3<T>& v) {
    return Vec3<T>(t * v.x(), t * v.y(), t * v.z());
}

template <typename T>
inline Vec3<T> operator*(const Vec3<T>& v, T t) {
    return t * v;
}

template <typename T>
inline Vec3<T> operator/(const Vec3<T>& v, T t) {
    return (1 / t) * v;
}

template <typename T>
inline T dot(const Vec3<T>& u, const Vec3<T>& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <typename T>
inline Vec3<T> cross(const Vec3<T>& u, const Vec3<T>& v) {
    return Vec3<T>(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                   u.x() * v.y() - u.y() * v.x());
}

template <typename T>
inline Vec3<T> unit_vector(const Vec3<T>& v) {
    return v / v.length();
}

}  // namespace geometry

#endif  // VEC3_H