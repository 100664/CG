// cone.hpp
#include <string>
#include <vector>
#include <cmath>
#ifndef CONE_H
#define CONE_H

#include "../utils/figure.hpp"
#include "../utils/triangle.hpp"

FIGURE generate_cone(float bottom_radius, float height, int slices, int stacks);

POINT calcula_pontos(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual);

std::vector<TRIANGLE> PAT (float radius, float height, int slices, int stacks, int slice_atual, int stack_atual);

std::vector<TRIANGLE> merge_vectors (std::vector<TRIANGLE> v1, std::vector<TRIANGLE> v2);

std::vector<TRIANGLE> triangles_sort(std::vector<POINT> points);
#endif // CONE_H