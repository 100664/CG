// triangle.hpp
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.hpp"
#include <stdio.h>
#include <vector>
#include <memory>
#include <string>
#include <cstdio>

typedef struct triangle *TRIANGLE;

void add_vertex(TRIANGLE t, POINT p);

TRIANGLE create_triangle();

TRIANGLE create_triangle_with_vertices (std::vector<POINT> vertices);

std::vector<POINT>* get_points (TRIANGLE t);

int verify_triangle (TRIANGLE t);

void print_triangle (TRIANGLE t);

void free_triangle (TRIANGLE t);


#endif // TRIANGLE_H