#include "matrix.hpp"
#include <stdlib.h>

using namespace std;

typedef struct matrix {
    int rows;
    int cols;
    float **values;
} *MATRIX;

MATRIX create_matrix(int rows, int cols, float **values) {
    MATRIX m = (MATRIX)malloc(sizeof(MATRIX));
    m->rows = rows;
    m->cols = cols;
    m->values = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        m->values[i] = (float*)malloc(cols * sizeof(float));
        if (values != NULL) {
            for (int j = 0; j < cols; j++) {
                m->values[i][j] = values[i][j];
            }
        }
    }
    return m;
}

void free_matrix(MATRIX m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->values[i]);
    }
    free(m->values);
    free(m);
}

MATRIX matrix_multiplication(MATRIX a, MATRIX b) {
    if (a->cols != b->rows) {
        return NULL;
    }
    MATRIX result = create_matrix(a->rows, b->cols, NULL);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->values[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->values[i][j] += a->values[i][k] * b->values[k][j];
            }
        }
    }
    return result;
}

float dot_product(MATRIX a, MATRIX b) {
    float result = 0.0f;
    for (int i = 0; i < a->rows; ++i) {
        for (int j = 0; j < a->cols; ++j) {
            result += a->values[i][j] * b->values[i][j];
        }
    }
    return result;
}

float** copy_values(float **matrix, int rows, int cols) {
    float **copy = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        copy[i] = (float*)malloc(cols * sizeof(float));
        for (int j = 0; j < cols; j++) {
            copy[i][j] = matrix[i][j];
        }
    }
    return copy;
}

void print_matrix(MATRIX m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            printf("%.2f ", m->values[i][j]);
        }
        printf("\n");
    }
}


void buildRotMatrix(POINT x, POINT y, POINT z, float* result) {
    result[0] = get_X(x);  result[4] = get_Y(x);  result[8]  = get_Z(x);  result[12] = 0.0f;
    result[1] = get_X(y);  result[5] = get_Y(y);  result[9]  = get_Z(y);  result[13] = 0.0f;
    result[2] = get_X(z);  result[6] = get_Y(z);  result[10] = get_Z(z);  result[14] = 0.0f;
    result[3] = 0.0f ;     result[7] = 0.0f;      result[11] = 0.0f;      result[15] = 1.0f;
}