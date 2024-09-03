// catmull.hpp
#ifndef CATMULL_H
#define CATMULL_H

#include "../utils/point.hpp"
#include "../utils/matrix.hpp"

void getCatmullRomPoint(float t, POINT p0, POINT p1, POINT p2, POINT p3, float * pos, float * deriv);

void getGlobalCatmullRomPoint(float gt, std::vector<POINT> control_points, float * pos, float * deriv);

#endif // CATMULL_H