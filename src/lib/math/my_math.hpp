#ifndef MY_MATH_HPP
#define MY_MATH_HPP

#include <cmath>
#include <cstdlib>
#include <memory>
#include <random>

namespace my_math {

using fp = double;

constexpr fp F_ZERO = static_cast<fp>(0.0);
constexpr fp F_ONE = static_cast<fp>(1.0);
constexpr fp F_NEG_ONE = static_cast<fp>(-1.0);
constexpr fp F_TWO = static_cast<fp>(2.0);
constexpr fp F_HALF = static_cast<fp>(0.5);
constexpr fp F_INF = std::numeric_limits<fp>::infinity();
constexpr fp F_PI = static_cast<fp>(3.1415926535897932385);
constexpr fp F_180 = static_cast<fp>(180.0);
constexpr fp F_EPS = static_cast<fp>(1e-3);
constexpr fp F_EPS_2 = static_cast<fp>(1e-8);

inline fp degrees_to_radians(fp degrees) { return degrees * F_PI / F_180; }

class RandomGenerator {
   public:
    // Delete copy constructor and assignment operator to enforce singleton
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    // Accessor for the singleton instance
    static RandomGenerator& getInstance() {
        static RandomGenerator instance;  // Guaranteed to be initialized only once
        return instance;
    }

    fp generate() { return distribution(generator); }

   private:
    RandomGenerator() : generator(std::random_device{}()), distribution(F_ZERO, F_ONE) {}

    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution;
};

inline fp random_number() { return RandomGenerator::getInstance().generate(); }

inline fp random_number(fp min, fp max) { return min + (max - min) * random_number(); }

inline int32_t random_integer(int32_t min, int32_t max) {
    return static_cast<int32_t>(random_number(min, max + 1));
}

}  // namespace my_math

// Register namespace

namespace geometry {
using namespace my_math;
}  // namespace geometry

namespace image {
using namespace my_math;
}  // namespace image

namespace world {
using namespace my_math;
}  // namespace world

namespace gui {
using namespace my_math;
}  // namespace gui

#endif  // MY_MATH_HPP