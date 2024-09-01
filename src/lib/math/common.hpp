#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP

#include "src/lib/math/vec3.hpp"

namespace geometry {

// Common vector types

using vec3 = Vec3<fp>;

using point3 = vec3;

// Vector utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(fp t, const vec3& v) { return vec3(t * v.x(), t * v.y(), t * v.z()); }

inline vec3 operator*(const vec3& v, fp t) { return t * v; }

inline vec3 operator/(const vec3& v, fp t) { return (1 / t) * v; }

inline fp dot(const vec3& u, const vec3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

inline vec3 unit_vector(const vec3& v) { return v / v.length(); }

inline vec3 random_vec3() { return vec3(random_number(), random_number(), random_number()); }

inline vec3 random_vec3(fp min, fp max) {
    return vec3(random_number(min, max), random_number(min, max), random_number(min, max));
}

// TODO: optimize
inline vec3 random_vec3_in_unit_disk() {
    while (true) {
        auto p = vec3(random_number(F_NEG_ONE, F_ONE), random_number(F_NEG_ONE, F_ONE), F_ZERO);
        if (p.length2() < F_ONE) return p;
    }
}

// TODO: optimize
inline vec3 random_vec3_in_unit_sphere() {
    while (true) {
        auto p = random_vec3(F_NEG_ONE, F_ONE);
        if (p.length2() < F_ONE) return p;
    }
}

inline vec3 random_unit_vec3() { return unit_vector(random_vec3_in_unit_sphere()); }

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vec3();
    if (dot(on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2 * dot(v, n) * n; }

/// @brief Calculates refraction ray.
/// @param uv Unit vector representing input ray.
/// @param n Unit vector representing normal.
/// @param relative_refraction_index Relative refraction index of two materials.
/// @return Refracted rec vector.
inline vec3 refract(const vec3& uv, const vec3& n, fp relative_refraction_index) {
    auto cos_theta = std::fmin(dot(-uv, n), F_ONE);
    vec3 r_out_perp = relative_refraction_index * (uv + cos_theta * n);
    vec3 r_out_para = -std::sqrt(std::fabs(F_ONE - r_out_perp.length2())) * n;
    return r_out_para + r_out_perp;
}

}  // namespace geometry

#endif  // COMMON_TYPES_HPP