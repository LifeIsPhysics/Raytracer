#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>


// C++ STD redefinitions
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Mathematical constants
const double inf = std::numeric_limits<double>::infinity();
const double pi  = 3.1415926535897932385;

// Utility functions
inline double degree_to_radians(double degree) {
  return degree * pi / 180.0;
}

inline double random_double() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

// Common headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif /* RTWEEKEND_H */