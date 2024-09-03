#define GL_SILENCE_DEPRECATION
#ifndef FIGURE_HPP
#define FIGURE_HPP


#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include "point.hpp" // Inclua o arquivo de cabeçalho do ponto aqui
#include "triangle.hpp"
#include "figure.hpp"

using namespace std;

// Definição completa do enum FIGURE_TYPE
enum FIGURE_TYPE {
    UNKNOWN,
    SPHERE,
    CONE,
    PLANE,
    BOX,
    RING,
    BEZIER
};

typedef struct figure *FIGURE;

std::vector<TRIANGLE>* get_triangles(FIGURE figure);

std::vector<POINT>* get_normals(FIGURE figure);

std::vector<POINT>* get_textures(FIGURE figure);

FIGURE create_figure_empty();

FIGURE create_figure_plane_box (FIGURE_TYPE type, int length, int divisions);

FIGURE create_figure_sphere (float radius, int slices, int stacks);

FIGURE create_figure_cone (float height, float radius, int slices, int stacks);

FIGURE create_figure_ring(float inner_radius, float outer_radius, int slices);

FIGURE create_figure_bezier(float tessellation, const char* patches_file);

void add_triangle(FIGURE f, TRIANGLE t);

void save_file(FIGURE f, std::string filename);

FIGURE fileToFigure(const std::string& filepath);

void add_triangles(FIGURE f, std::vector<TRIANGLE> t);

void print_figura_vertices(FIGURE f);

void printf_normais (FIGURE f);

void printf_texturas (FIGURE f);

int number_triangles(FIGURE f);

std::string print_triangulos(FIGURE f);

void free_figure (FIGURE f);

std::vector<float> figure_to_vectors(const FIGURE& figure);

void concat_FIGURES (FIGURE f1, FIGURE f2);

void add_normal(FIGURE f, POINT p);

void add_texture(FIGURE f, POINT p);

void add_normals(FIGURE f, const std::vector<POINT>& normals);

void add_textures(FIGURE f, const std::vector<POINT>& textures);

std::vector<float> figure_to_normals(FIGURE f);

std::vector<float> figure_to_textures(FIGURE f);

#endif /* FIGURE_HPP */
