#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

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

// Common headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif /* RTWEEKEND_H */