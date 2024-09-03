#include "sphere.hpp"

FIGURE generate_sphere(float radius, int slices, int stacks) {
    FIGURE sphere = create_figure_sphere(radius, slices, stacks);

    // Criar os triângulos laterais
    for (int i = 0; i <= stacks; ++i) {  // Adiciona uma iteração extra para o polo norte
        for (int j = 0; j < slices; ++j) {
            float phi = 2 * M_PI * j / slices;
            float theta = M_PI * i / (stacks + 1);
            float nextTheta = M_PI * (i + 1) / (stacks + 1);

            POINT p1, p2, p3, p4;
            p1 = new_point(radius * sin(theta) * cos(phi),
                           radius * cos(theta),
                           radius * sin(theta) * sin(phi));

            p2 = new_point(radius * sin(theta) * cos(phi + 2 * M_PI / slices),
                           radius * cos(theta),
                           radius * sin(theta) * sin(phi + 2 * M_PI / slices));

            p3 = new_point(radius * sin(nextTheta) * cos(phi),
                           radius * cos(nextTheta),
                           radius * sin(nextTheta) * sin(phi));

            p4 = new_point(radius * sin(nextTheta) * cos(phi + 2 * M_PI / slices),
                           radius * cos(nextTheta),
                           radius * sin(nextTheta) * sin(phi + 2 * M_PI / slices));

            std::vector<POINT> vertices1;
            std::vector<POINT> vertices2;

            vertices1.push_back(p1);
            vertices1.push_back(p2);
            vertices1.push_back(p3);
            vertices2.push_back(p2);
            vertices2.push_back(p4);
            vertices2.push_back(p3);

            TRIANGLE t1 = create_triangle_with_vertices(vertices1);
            TRIANGLE t2 = create_triangle_with_vertices(vertices2);

            add_triangle(sphere, t1);
            add_triangle(sphere, t2);

            // Adicione normais e texturas
            POINT normal1 = new_point(sin(theta) * cos(phi), cos(theta), sin(theta) * sin(phi));
            POINT normal2 = new_point(sin(nextTheta) * cos(phi), cos(nextTheta), sin(nextTheta) * sin(phi));
            POINT normal3 = new_point(sin(theta) * cos(phi + 2 * M_PI / slices), cos(theta), sin(theta) * sin(phi + 2 * M_PI / slices));
            POINT normal4 = new_point(sin(nextTheta) * cos(phi + 2 * M_PI / slices), cos(nextTheta), sin(nextTheta) * sin(phi + 2 * M_PI / slices));

            add_normal(sphere, normal1);
            add_normal(sphere, normal3);
            add_normal(sphere, normal2);

            add_normal(sphere, normal2);
            add_normal(sphere, normal3);
            add_normal(sphere, normal4);

            POINT texture1 = new_point((float)j / slices, (float)i / stacks, 0.0f);
            POINT texture2 = new_point((float)(j + 1) / slices, (float)i / stacks, 0.0f);
            POINT texture3 = new_point((float)j / slices, (float)(i + 1) / stacks, 0.0f);
            POINT texture4 = new_point((float)(j + 1) / slices, (float)(i + 1) / stacks, 0.0f);

            add_texture(sphere, texture1);
            add_texture(sphere, texture2);
            add_texture(sphere, texture3);
            add_texture(sphere, texture2);
            add_texture(sphere, texture4);
            add_texture(sphere, texture3);
        }
    }

    // Adicionar o polo norte
    std::vector<POINT> verticesPoloNorte;
    POINT poloNorte = new_point(0.0f, radius, 0.0f); // Centro do polo norte
    for (int j = 0; j < slices; ++j) {
        float phi = 2 * M_PI * j / slices;
        POINT p = new_point(radius * sin(M_PI / (stacks + 1)) * cos(phi),
                            radius * cos(M_PI / (stacks + 1)),
                            radius * sin(M_PI / (stacks + 1)) * sin(phi));
        verticesPoloNorte.push_back(p);
    }

    for (int j = 0; j < slices - 1; ++j) {
        std::vector<POINT> verticesTriangulo;
        verticesTriangulo.push_back(poloNorte);
        verticesTriangulo.push_back(verticesPoloNorte[j]);
        verticesTriangulo.push_back(verticesPoloNorte[j + 1]);
        TRIANGLE tPoloNorte = create_triangle_with_vertices(verticesTriangulo);
        add_triangle(sphere, tPoloNorte);

        // Adicione normais e texturas
        POINT normalPoloNorte = new_point(0.0f, 1.0f, 0.0f);
        add_normal(sphere, normalPoloNorte);
        add_normal(sphere, normalPoloNorte);
        add_normal(sphere, normalPoloNorte);

        POINT texturePoloNorte1 = new_point(0.5f, 1.0f, 0.0f);
        POINT texturePoloNorte2 = new_point((float)(j + 1) / slices, 1.0f, 0.0f);
        POINT texturePoloNorte3 = new_point((float)j / slices, 1.0f, 0.0f);

        add_texture(sphere, texturePoloNorte1);
        add_texture(sphere, texturePoloNorte2);
        add_texture(sphere, texturePoloNorte3);
    }
    // Adicionar o último triângulo conectando o último ponto ao primeiro
    std::vector<POINT> verticesTriangulo;
    verticesTriangulo.push_back(poloNorte);
    verticesTriangulo.push_back(verticesPoloNorte[slices - 1]);
    verticesTriangulo.push_back(verticesPoloNorte[0]);
    TRIANGLE tPoloNorte = create_triangle_with_vertices(verticesTriangulo);
    add_triangle(sphere, tPoloNorte);

    // Adicione normais e texturas
    POINT normalPoloNorte = new_point(0.0f, 1.0f, 0.0f);
    add_normal(sphere, normalPoloNorte);
    add_normal(sphere, normalPoloNorte);
    add_normal(sphere, normalPoloNorte);

    POINT texturePoloNorte1 = new_point(0.5f, 1.0f, 0.0f);
    POINT texturePoloNorte2 = new_point(1.0f, 1.0f, 0.0f);
    POINT texturePoloNorte3 = new_point(0.0f, 1.0f, 0.0f);

    add_texture(sphere, texturePoloNorte1);
    add_texture(sphere, texturePoloNorte2);
    add_texture(sphere, texturePoloNorte3);

    return sphere;
}
