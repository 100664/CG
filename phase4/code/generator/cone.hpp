// cone.hpp
#define GL_SILENCE_DEPRECATION
#include <string>
#include <vector>
#include <cmath>
#ifndef CONE_H
#define CONE_H

#include "../utils/figure.hpp"
#include "../utils/triangle.hpp"

POINT calcula_pontos(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual);

POINT calcula_normal(float radius, float height, int slices, int slice_atual);

POINT calcula_textura(int slices, int stacks, int slice_atual, int stack_atual);

std::vector<TRIANGLE> PAT(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual);

std::vector<TRIANGLE> triangles_sort(std::vector<POINT> points);

std::vector<TRIANGLE> merge_vectors(std::vector<TRIANGLE> v1, std::vector<TRIANGLE> v2);

FIGURE generate_cone(float radius, float height, int slices, int stacks);

#endif // CONE_H