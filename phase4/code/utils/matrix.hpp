// matrix.hpp
#define GL_SILENCE_DEPRECATION
#ifndef MATRIX_H
#define MATRIX_H


#include <stdlib.h>
#include <vector>
#include "point.hpp"

using namespace std;

typedef struct matrix  *MATRIX;

MATRIX create_matrix(int rows, int cols, float **values);

void free_matrix(MATRIX m);

MATRIX matrix_multiplication(MATRIX a, MATRIX b);

float dot_product(MATRIX a, MATRIX b);

float** copy_values(float **matrix, int rows, int cols);

void buildRotMatrix(POINT x, POINT y, POINT z, float* result);

#endif // MATRIX_H