// bezier.hpp
#ifndef BEZIER_H
#define BEZIER_H

#include "../utils/figure.hpp"
#include "../utils/point.hpp"

std::vector<std::vector<POINT> > parse_patch_file(const char *filePath);

POINT bezier_interpolate_row(std::vector<POINT> controlPoints, float t);

POINT bezier_interpolate(std::vector<POINT> patch, float u, float v);

std::vector<TRIANGLE> generate_bezier_surfaces(std::vector<POINT> patch, float tessellation);

FIGURE generate_bezier (float tessellation, const char* patches_file);

#endif // BEZIER_H