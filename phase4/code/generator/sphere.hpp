// sphere.hpp
#define GL_SILENCE_DEPRECATION
#ifndef SPHERE_H
#define SPHERE_H

#include "../utils/figure.hpp"
#include "../utils/point.hpp"
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

FIGURE generate_sphere(float radius, int slices, int stacks);

#endif // SPHERE_H