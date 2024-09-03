#include "cone.hpp"
#include <cmath>

using namespace std;

POINT calcula_pontos(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual) {
    double y = stack_atual * height / stacks;
    double atual_radius = (height - y) * (radius / height);
    double theta = 2 * M_PI * slice_atual / slices;
    double x = atual_radius * cos(theta);
    double z = atual_radius * sin(theta);
    return new_point(x, y, z);
}

POINT calcula_normal(float radius, float height, int slices, int slice_atual) {
    double theta = 2 * M_PI * slice_atual / slices;
    double x = cos(theta);
    double z = sin(theta);
    
    // A inclinação da superfície lateral do cone é dada pela relação entre altura e raio.
    // Normal não normalizada
    double nx = x * height;
    double ny = radius;
    double nz = z * height;

    POINT normal = new_point(nx, ny, nz);
    normalize(normal);
    return normal;
}

POINT calcula_textura(int slices, int stacks, int slice_atual, int stack_atual) {
    float u = (float) slice_atual / slices;
    float v = (float) stack_atual / stacks;
    return new_point(u, v, 0);
}

std::vector<TRIANGLE> PAT(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual) {
    std::vector<POINT> pontos;

    if (stack_atual == 0) {
        POINT p1 = new_point(0, 0, 0);
        POINT p2 = calcula_pontos(radius, height, slices, stacks, slice_atual, 1);
        POINT p3 = calcula_pontos(radius, height, slices, stacks, (slice_atual + 1) % slices, 1);
        pontos.push_back(p1);
        pontos.push_back(p2);
        pontos.push_back(p3);
    } else if (stack_atual == stacks) {
        POINT p1 = calcula_pontos(radius, height, slices, stacks, slice_atual, stacks - 1);
        POINT p2 = new_point(0, height, 0);
        POINT p3 = calcula_pontos(radius, height, slices, stacks, (slice_atual + 1) % slices, stacks - 1);
        pontos.push_back(p1);
        pontos.push_back(p2);
        pontos.push_back(p3);
    } else {
        POINT p1 = calcula_pontos(radius, height, slices, stacks, slice_atual, stack_atual);
        POINT p2 = calcula_pontos(radius, height, slices, stacks, slice_atual, stack_atual + 1);
        POINT p3 = calcula_pontos(radius, height, slices, stacks, (slice_atual + 1) % slices, stack_atual + 1);
        POINT p4 = calcula_pontos(radius, height, slices, stacks, (slice_atual + 1) % slices, stack_atual);
        pontos.push_back(p1);
        pontos.push_back(p2);
        pontos.push_back(p3);
        pontos.push_back(p4);
    }

    std::vector<TRIANGLE> triangles = triangles_sort(pontos);
    return triangles;
}

std::vector<TRIANGLE> triangles_sort(std::vector<POINT> points) {
    std::vector<TRIANGLE> triangles;
    if (points.size() == 3) {
        TRIANGLE t = create_triangle();
        add_vertex(t, points[0]);
        add_vertex(t, points[1]);
        add_vertex(t, points[2]);
        triangles.push_back(t);
    } else if (points.size() == 4) {
        TRIANGLE t1 = create_triangle();
        add_vertex(t1, points[0]);
        add_vertex(t1, points[1]);
        add_vertex(t1, points[2]);
        triangles.push_back(t1);
        TRIANGLE t2 = create_triangle();
        add_vertex(t2, points[0]);
        add_vertex(t2, points[2]);
        add_vertex(t2, points[3]);
        triangles.push_back(t2);
    }
    return triangles;
}

std::vector<TRIANGLE> merge_vectors(std::vector<TRIANGLE> v1, std::vector<TRIANGLE> v2) {
    std::vector<TRIANGLE> result;
    result.reserve(v1.size() + v2.size());
    result.insert(result.end(), v1.begin(), v1.end());
    result.insert(result.end(), v2.begin(), v2.end());
    return result;
}

FIGURE generate_cone(float radius, float height, int slices, int stacks) {
    FIGURE f = create_figure_cone(height, radius, slices, stacks);
    std::vector<TRIANGLE> triangles;
    std::vector<POINT> textures;
    std::vector<POINT> normais;

    for (int stack_atual = 0; stack_atual <= stacks; stack_atual++) {
        for (int slice_atual = 0; slice_atual < slices; slice_atual++) {
            std::vector<TRIANGLE> t = PAT(radius, height, slices, stacks, slice_atual, stack_atual);
            triangles = merge_vectors(triangles, t);

            if (stack_atual == 0) {
                // Normais para a base do cone
                normais.push_back(new_point(0, -1, 0));
                normais.push_back(new_point(0, -1, 0));
                normais.push_back(new_point(0, -1, 0));
            } else if (stack_atual == stacks) {
                // Normais para o vértice do cone
                normais.push_back(new_point(0, 1, 0));
                normais.push_back(new_point(0, 1, 0));
                normais.push_back(new_point(0, 1, 0));
            } else {
                // Normais para a superfície lateral do cone
                POINT normal = calcula_normal(radius, height, slices, slice_atual);
                normais.push_back(normal);
                normais.push_back(normal);
                normais.push_back(normal);
                normais.push_back(normal);
            }

            textures.push_back(calcula_textura(slices, stacks, slice_atual, stack_atual));
            textures.push_back(calcula_textura(slices, stacks, slice_atual, stack_atual));
            textures.push_back(calcula_textura(slices, stacks, slice_atual, stack_atual));
            if (stack_atual != 0 && stack_atual != stacks) {
                textures.push_back(calcula_textura(slices, stacks, slice_atual, stack_atual));
            }
        }
    }

    add_triangles(f, triangles);
    add_normals(f, normais);
    add_textures(f, textures);

    return f;
}
