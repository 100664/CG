#include "ring.hpp"

FIGURE generate_ring(float inner_radius, float outer_radius, int slices) {
    if (inner_radius >= outer_radius) {
        fprintf(stderr, "Erro: Raio interno maior ou igual ao raio externo.\n");
        return nullptr;
    }

    if (slices < 3) {
        fprintf(stderr, "Erro: Número de slices deve ser pelo menos 3.\n");
        return nullptr;
    }

    FIGURE ring = create_figure_ring(inner_radius, outer_radius, slices);

    float delta_angle = 2 * M_PI / slices;
    float angle = 0.0f;

    for (int i = 0; i < slices; ++i) {
        float x1 = outer_radius * cos(angle);
        float z1 = outer_radius * sin(angle);
        float x2 = outer_radius * cos(angle + delta_angle);
        float z2 = outer_radius * sin(angle + delta_angle);
        float x3 = inner_radius * cos(angle);
        float z3 = inner_radius * sin(angle);
        float x4 = inner_radius * cos(angle + delta_angle);
        float z4 = inner_radius * sin(angle + delta_angle);

        POINT p1 = new_point(x1, 0.0f, z1);
        POINT p2 = new_point(x2, 0.0f, z2);
        POINT p3 = new_point(x3, 0.0f, z3);
        POINT p4 = new_point(x4, 0.0f, z4);

        TRIANGLE t1 = create_triangle();
        TRIANGLE t2 = create_triangle();

        add_vertex(t1, p1);
        add_vertex(t1, p3);
        add_vertex(t1, p2);

        add_vertex(t2, p2);
        add_vertex(t2, p3);
        add_vertex(t2, p4);

        add_triangle(ring, t1);
        add_triangle(ring, t2);

        angle += delta_angle;
    }

    return ring;
}
