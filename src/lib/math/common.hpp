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

inline void vec3_add(const vec3& u, const vec3& v, vec3& result) {
    result[0] = u[0] + v[0];
    result[1] = u[1] + v[1];
    result[2] = u[2] + v[2];
}

inline void vec3_sub(const vec3& u, const vec3& v, vec3& result) {
    result[0] = u[0] - v[0];
    result[1] = u[1] - v[1];
    result[2] = u[2] - v[2];
}

inline void vec3_elementwise_mul(const vec3& u, const vec3& v, vec3& result) {
    result[0] = u.x() * v.x();
    result[1] = u.y() * v.y();
    result[2] = u.z() * v.z();
}

inline void vec3_scalar_mul(fp t, const vec3& v, vec3& result) {
    result[0] = t * v[0];
    result[1] = t * v[1];
    result[2] = t * v[2];
}

inline fp vec3_dot(const vec3& u, const vec3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline void vec3_cross_product(const vec3& u, const vec3& v, vec3& result) {
    result[0] = u.y() * v.z() - u.z() * v.y();
    result[1] = u.z() * v.x() - u.x() * v.z();
    result[2] = u.x() * v.y() - u.y() * v.x();
}

inline void vec3_unit(const vec3& v, vec3& result) {
    vec3_scalar_mul(F_ONE / v.length(), v, result);
}

inline vec3 random_vec3() { return vec3(random_number(), random_number(), random_number()); }

inline vec3 random_vec3(fp min, fp max) {
    return vec3(random_number(min, max), random_number(min, max), random_number(min, max));
}

inline void random_vec3_in_unit_disk(vec3& result) {
    result = vec3(random_number(F_NEG_ONE, F_ONE), random_number(F_NEG_ONE, F_ONE), F_ZERO);
    vec3_unit(result, result);
    auto d = std::cbrt(random_number(F_ZERO, F_ONE));
    vec3_scalar_mul(d, result, result);
}

inline void random_vec3_in_unit_sphere(vec3& result) {
    result = random_vec3(F_NEG_ONE, F_ONE);
    vec3_unit(result, result);
    auto d = std::cbrt(random_number(F_ZERO, F_ONE));
    vec3_scalar_mul(d, result, result);
}

inline void random_unit_vec3(vec3& result) {
    result = random_vec3(F_ZERO, F_ONE);  // TODO: optimize memory
    vec3_unit(result, result);
}

inline void random_on_hemisphere(const vec3& normal, vec3& result) {
    random_unit_vec3(result);  // TODO: optimize memory
    if (!(vec3_dot(result, normal) > 0.0)) {
        vec3_scalar_mul(F_NEG_ONE, result, result);
    }
}

inline void vec3_reflect(const vec3& v, const vec3& n, vec3& result) {
    vec3_scalar_mul(-2 * vec3_dot(v, n), n, result);  // result = - 2 * vec3_dot(v, n) * n
    result += v;                                      // result = v - 2 * vec3_dot(v, n) * n
}

inline void vec3_refract(const vec3& uv, const vec3& n, fp relative_refraction_index,
                         vec3& result) {
    vec3 temp{};
    vec3_scalar_mul(F_NEG_ONE, uv, temp);
    auto cos_theta = std::fmin(vec3_dot(temp, n), F_ONE);
    vec3_scalar_mul(cos_theta, n, result);
    result += uv;
    result *= relative_refraction_index;

    vec3_scalar_mul(-std::sqrt(std::fabs(F_ONE - result.length2())), n, temp);
    result += temp;
}

}  // namespace geometry

#endif  // COMMON_TYPES_HPP