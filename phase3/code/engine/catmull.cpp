#include "catmull.hpp"

void getCatmullRomPoint(float t, POINT p0, POINT p1, POINT p2, POINT p3, float *pos, float *deriv) {
    // Matriz de Catmull-Rom
    float m[4][4] = {
        {-0.5f,  1.5f, -1.5f,  0.5f},
        { 1.0f, -2.5f,  2.0f, -0.5f},
        {-0.5f,  0.0f,  0.5f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f}
    };

    float p[4][3] = {
        {get_X(p0), get_Y(p0), get_Z(p0)},
        {get_X(p1), get_Y(p1), get_Z(p1)}, 
        {get_X(p2), get_Y(p2), get_Z(p2)},
        {get_X(p3), get_Y(p3), get_Z(p3)}
    };
                
    // Calcula A = M * P
    float a[4][3];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            a[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                a[i][j] += m[i][k] * p[k][j];
            }
        }
    }

    // Calcula pos = T * A
    if(pos != nullptr) {
        float T[4] = {t * t * t, t * t, t, 1};
        for (int i = 0; i < 3; i++) {
            pos[i] = 0;
            for (int j = 0; j < 4; j++) {
                pos[i] += T[j] * a[j][i];
            }
        }
    }
    
    // Calcula deriv = T' * A
    if (deriv != nullptr) {
        float T_[4] = {3 * t * t, 2 * t, 1, 0};
        for (int i = 0; i < 3; i++) {
            deriv[i] = 0;
            for (int j = 0; j < 4; j++) {
                deriv[i] += T_[j] * a[j][i];
            }
        }
    }
}

void getGlobalCatmullRomPoint(float gt, std::vector<POINT> control_points, float *pos, float *deriv) {
    size_t POINT_COUNT = control_points.size();
    float t = gt * POINT_COUNT; // T verdadeiro global
    int index = floor(t);             // Qual segmento
    t = t - index;                    // Onde dentro do segmento

    // Índices armazenam os pontos
    int indices[4];
    indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
    indices[1] = (indices[0] + 1) % POINT_COUNT;
    indices[2] = (indices[1] + 1) % POINT_COUNT;
    indices[3] = (indices[2] + 1) % POINT_COUNT;

    // Calcula ponto de Catmull-Rom e derivada
    getCatmullRomPoint(t, control_points[indices[0]], control_points[indices[1]], control_points[indices[2]], control_points[indices[3]], pos, deriv);
}
