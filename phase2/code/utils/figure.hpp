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

// Definição completa do enum FIGURE_TYPE
enum FIGURE_TYPE {
    UNKNOWN,
    SPHERE,
    CONE,
    PLANE,
    BOX,
    RING
};

typedef struct figure *FIGURE;

std::vector<TRIANGLE>* get_triangles(FIGURE figure);

FIGURE create_figure_empty();
FIGURE create_figure_plane_box (FIGURE_TYPE type, int length, int divisions);
FIGURE create_figure_sphere (float radius, int slices, int stacks);
FIGURE create_figure_cone (float height, float radius, int slices, int stacks);
FIGURE create_figure_ring(float inner_radius, float outer_radius, int slices);
void add_triangle(FIGURE f, TRIANGLE t);
void save_file(FIGURE f, std::string filename);
FIGURE fileToFigure(const std::string& filename);
void add_triangles(FIGURE f, std::vector<TRIANGLE> t);
//void concatenateAndRemoveDuplicates(FIGURE f, FIGURE f2);
//bool comparePoints(POINT a, POINT b);
void print_figura(FIGURE f);
int number_triangles(FIGURE f);
std::string print_triangulos(FIGURE f);
void free_figure (FIGURE f);

void concat_FIGURES (FIGURE f1, FIGURE f2);

#endif /* FIGURE_HPP */
