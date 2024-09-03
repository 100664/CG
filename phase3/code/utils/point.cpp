#include "point.hpp"


typedef struct point {
    float x;
    float y;
    float z;
} *POINT;

//Pode vir a ser preciso criar um ponto sem que se saiba as suas coordenadas

POINT new_null_point () {
    return new_point(0.0f, 0.0f, 0.0f);
}

POINT new_point (float x, float y, float z) {
    POINT p = (POINT)malloc (sizeof(struct point));
    if (p != NULL){
        p->x = x;
        p->y = y;
        p->z = z;
    } 
    return p;
}
 

float get_X (POINT p) {
    if (p != NULL)
        return p->x;
    return 0.0f;
}

float get_Y (POINT p) {
    if (p != NULL)
        return p->y;
    return 0.0f;
}

float get_Z (POINT p) {
    if (p != NULL)
        return p->z;
    return 0.0f;
}

void set_X (POINT p, float x) {
        p->x = x;
}

void set_Y (POINT p, float y) {
        p->y = y;
}

void set_Z (POINT p, float z) {
        p->z = z;
}

float distance_Point_to_origin(POINT p) {
    return sqrt((get_X(p) * get_X(p)) + (get_Y(p) * get_Y(p)) + (get_Z(p) * get_Z(p)));
}

float distance_Point_to_Point (POINT p1, POINT p2){
    return sqrt((get_X(p1) - get_X(p2)) * (get_X(p1) - get_X(p2)) + (get_Y(p1) - get_Y(p2)) * (get_Y(p1) - get_Y(p2)) + (get_Z(p1) - get_Z(p2)) * (get_Z(p1) - get_Z(p2)));
}

POINT dup_point(POINT p){
    return new_point(get_X(p), get_Y(p), get_Z(p));
}

void print_point(POINT p){
    printf("X: %f, Y: %f, Z: %f\n", get_X(p), get_Y(p), get_Z(p));
}

void free_point(POINT p){
    free(p);
}

bool equals_point(POINT p1, POINT p2){
    return (get_X(p1) == get_X(p2) && get_Y(p1) == get_Y(p2) && get_Z(p1) == get_Z(p2));
}

POINT calculate_bezier_point(const std::vector<POINT>& control_points, float u) {
    int n = control_points.size() - 1; // Grau do polinômio de Bernstein
    float px = 0.0f;
    float py = 0.0f;
    float pz = 0.0f;

    for (int i = 0; i <= n; ++i) {
        float bi = 1.0f;
        for (int j = 0; j <= n; ++j) {
            if (i != j) {
                bi *= (u - j) / (i - j);
            }
        }
        px += control_points[i]->x * bi;
        py += control_points[i]->y * bi;
        pz += control_points[i]->z * bi;
    }

    return new_point(px, py, pz);
}

void cross(const POINT a, const POINT b, POINT &result) {
    result = new_point(get_Y(a) * get_Z(b) - get_Z(a) * get_Y(b),
                       get_Z(a) * get_X(b) - get_X(a) * get_Z(b),
                       get_X(a) * get_Y(b) - get_Y(a) * get_X(b));
}

void normalize(POINT &z) {
    float norm = sqrt(get_X(z) * get_X(z) + get_Y(z) * get_Y(z) + get_Z(z) * get_Z(z));
    set_X(z, get_X(z) / norm);
    set_Y(z, get_Y(z) / norm);
    set_Z(z, get_Z(z) / norm);
}