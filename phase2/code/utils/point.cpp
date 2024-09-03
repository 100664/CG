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
    if (p != NULL)
        p->x = x;
}

void set_Y (POINT p, float y) {
    if (p != NULL)
        p->y = y;
}

void set_Z (POINT p, float z) {
    if (p != NULL)
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
