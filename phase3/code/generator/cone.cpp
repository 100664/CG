#include "cone.hpp"

using namespace std;

POINT calcula_pontos(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual){
    double y = (stack_atual-1) * height/stacks;
    radius = (height-y) * (radius / height);
    double theta = 2 * M_PI * slice_atual / slices; // Calcula o ângulo em radianos
    double x = radius * cos(theta); // Coordenada x
    double z = radius * sin(theta); // Coordenada z
    POINT p = new_point(x, y, z);
    return p;
}

//pontos adjacentes trianguláveis
std::vector<TRIANGLE> PAT(float radius, float height, int slices, int stacks, int slice_atual, int stack_atual){
    std::vector<POINT> pontos = std::vector<POINT>();
    if (slice_atual == 0 && stack_atual == 0)
    {   
        POINT p1 = new_point(0, 0, 0);
        POINT p2 = calcula_pontos(radius, height, slices, stacks, 1, 1);
        POINT p3 = calcula_pontos(radius, height, slices, stacks, 2, 1);
        POINT p4 = calcula_pontos(radius, height, slices, stacks, 3, 1);
        POINT p5 = calcula_pontos(radius, height, slices, stacks, 4, 1);
        pontos.push_back(p1);
        pontos.push_back(p2);
        pontos.push_back(p3);
        pontos.push_back(p4);
        pontos.push_back(p5);
    }
    else if (stack_atual == stacks)
    {
        POINT p1 = calcula_pontos(radius, height, slices, stacks, slice_atual, stack_atual);
        POINT p2 = calcula_pontos(radius, height, slices, stacks, slice_atual, stack_atual + 1);
        POINT p3 = calcula_pontos(radius, height, slices, stacks, slice_atual + 1, stack_atual + 1);
        POINT p4 = calcula_pontos(radius, height, slices, stacks, slice_atual + 1, stack_atual);
        pontos.push_back(p1);
        pontos.push_back(p2);
        pontos.push_back(p3);
        pontos.push_back(p4);
    }
    else
    {
        POINT p1 = calcula_pontos(radius, height, slices, stacks, slice_atual, stack_atual);
        POINT p2 = calcula_pontos(radius, height, slices, stacks, slice_atual, stack_atual + 1);
        POINT p3 = calcula_pontos(radius, height, slices, stacks, slice_atual + 1, stack_atual + 1);
        POINT p4 = calcula_pontos(radius, height, slices, stacks, slice_atual + 1, stack_atual);
        pontos.push_back(p1);
        pontos.push_back(p2);
        pontos.push_back(p3);
        pontos.push_back(p4);
    }
    std::vector<TRIANGLE> triangles = triangles_sort(pontos);
    return triangles;
}

std::vector<TRIANGLE> merge_vectors (std::vector<TRIANGLE> v1, std::vector<TRIANGLE> v2) {
     std::vector<TRIANGLE> result = std::vector<TRIANGLE>(); 
    result.reserve(v1.size() + v2.size()); //reserva espaço para a inserção
    result.insert(result.end(), v1.begin(), v1.end()); //insere os elementos do primeiro vetor
    result.insert(result.end(), v2.begin(), v2.end()); //insere os elementos do segundo vetor
    return result;
}

std::vector<TRIANGLE> triangles_sort(std::vector<POINT> points) {
    std::vector<TRIANGLE> triangles = std::vector<TRIANGLE>();
    POINT p1 = new_point(0,0,0);
    if (equals_point(points[0], p1) == 1){
        TRIANGLE t = create_triangle();
        add_vertex(t, points[0]);
        add_vertex(t, points[1]);     // Ponto atual no loop
        add_vertex(t, points[4]); // Próximo ponto subsequente
        triangles.push_back(t);
    }
    // Loop começa em 1, pois o primeiro ponto (points[0]) é sempre usado
    for (size_t i = 1; i < points.size() - 1; i++) {
        // Cria um triângulo com o primeiro ponto do vetor e cada par de pontos seguintes
        TRIANGLE t = create_triangle();
        add_vertex(t, points[0]);     // Primeiro ponto do vetor, sempre utilizado
        add_vertex(t, points[i]);     // Ponto atual no loop
        add_vertex(t, points[i + 1]); // Próximo ponto subsequente
        triangles.push_back(t);
    }
    return triangles;
}


FIGURE generate_cone(float radius, float height, int slices, int stacks){
    FIGURE f = create_figure_cone(height, radius, slices, stacks);
    std::vector<TRIANGLE> triangles = PAT(radius, height, slices, stacks, 0, 0);
    for (int stack_atual = 1; stack_atual <= stacks; stack_atual++){
        for (int slice_atual = 1; slice_atual <= slices; slice_atual++){
            triangles = merge_vectors(triangles, PAT(radius, height, slices, stacks, slice_atual, stack_atual));
        }
    }

    add_triangles(f, triangles);
    
    return f;
}