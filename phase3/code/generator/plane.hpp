// plane.hpp
#ifndef PLANE_H
#define PLANE_H

#include "../utils/figure.hpp"
#include "../utils/point.hpp"
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>

FIGURE generate_plane_XZ(int length, int divisions, float h = 0.0f, int baixo = 0);

FIGURE generate_plane_XY(int length, int divisions, float h = 0.0f, int baixo = 0);

FIGURE generate_plane_YZ(int length, int divisions, float h = 0.0f, int baixo = 0);


#endif // PLANE_H