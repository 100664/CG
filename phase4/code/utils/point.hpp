//não acho que haja nada a acrescentar
#define GL_SILENCE_DEPRECATION
// point.hpp
#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>

typedef struct point *POINT;

POINT new_null_point();

POINT new_point (float x, float y, float z);

float get_X (POINT p);

float get_Y (POINT p);

float get_Z (POINT p);

void set_X (POINT p, float x);

void set_Y (POINT p, float y);

void set_Z (POINT p, float z);

float distance_Point_to_origin(POINT p);

float distance_Point_to_Point(POINT p1, POINT p2);

POINT dup_point(POINT p);

void print_point(POINT p);

void free_point(POINT p);

bool equals_point(POINT p1, POINT p2);

POINT calculate_bezier_point(const std::vector<POINT>& control_points, float u);

void cross(const POINT a, const POINT b, POINT &result);

void normalize(POINT &z);

POINT subtract_points(POINT p1, POINT p2);

void safe_normalize(POINT& p);

#endif // POINT_H
