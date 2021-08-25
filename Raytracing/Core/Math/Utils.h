//
//  Utils.h
//  Raytracing
//
//  Created by Kashyap Rajpal on 3/26/20.
//  Copyright © 2020 Kashyap Rajpal. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <functional>
#include <random>

inline double RandomDouble()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator =
        std::bind(distribution, generator);
    return rand_generator();
}

inline double Clamp(double x, double min, double max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double RandomDouble(double min, double max)
{
    return Clamp(RandomDouble(), min, max);
}

// Usings

using std::shared_ptr;
using std::make_shared;

// Constants

static const double s_kInfinity = std::numeric_limits<double>::infinity();
static const double s_kPI = 3.1415926535897932385;

// Utility Functions

inline double DegreesToRadian(double degrees) {
    return degrees * s_kPI / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

#endif /* Utils_h */
