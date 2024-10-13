#ifndef OPENMP_HPP
#define OPENMP_HPP

#ifdef _OPENMP
#include <omp.h>

#define OMP_PARALLEL _Pragma("omp parallel")

#define OMP_FOR _Pragma("omp for")

#define OMP_PARALLEL_FOR_DYNAMIC _Pragma("omp parallel for num_threads(6) schedule(dynamic, 20)")

#else

#define OMP_PARALLEL
#define OMP_FOR
#define OMP_PARALLEL_FOR_DYNAMIC

#endif

#endif  // OPENMP_HPP
