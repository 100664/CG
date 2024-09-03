#define GL_SILENCE_DEPRECATION
#include "triangle.hpp"


typedef struct triangle {
    int num_points; // número de pontos
    std::vector<POINT>* vertices;
}*TRIANGLE;

void add_vertex(TRIANGLE t, POINT p) {
    if (t != NULL && t->num_points != 3) {  
        t->vertices->push_back(p);
        t->num_points += 1;
    }
}

TRIANGLE create_triangle_with_vertices (std::vector<POINT> vertices) {
    TRIANGLE t = (TRIANGLE)malloc(sizeof(struct triangle));
    t->num_points = 0;
    t->vertices = new std::vector<POINT>();
    for (POINT p : vertices)
        add_vertex(t, p);
    return t;
}

TRIANGLE create_triangle() {
    TRIANGLE t = (TRIANGLE)malloc(sizeof(struct triangle));
    t->num_points = 0;
    t->vertices = new std::vector<POINT>();
    return t;
}

int verify_triangle (TRIANGLE t) {
    if (t->num_points != 3)
        return 0;
    else
        return 1;
}

std::vector<POINT>* get_points (TRIANGLE t) {
    return t->vertices;
}

void print_triangle (TRIANGLE t) {
    if (t != NULL && t->vertices != NULL)
        for (POINT p : *(t->vertices))
            print_point(p);
}

void free_triangle (TRIANGLE t) {
    for (POINT p : *(t->vertices))
        free_point(p);
    free(t->vertices);
}
