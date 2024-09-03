// ring.hpp
#define GL_SILENCE_DEPRECATION
#ifndef RING_H
#define RING_H

#include "../utils/figure.hpp"
#include "../utils/point.hpp"
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

FIGURE generate_ring(float inner_radius, float outer_radius, int slices);

#endif // RING_H