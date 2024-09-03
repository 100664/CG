#define GL_SILENCE_DEPRECATION
#include "figure.hpp"


using namespace std;

typedef struct figure {
    FIGURE_TYPE type;
    std::vector<TRIANGLE>* triangles;
    union {
        struct {
            float radius;
            int slices;
            int stacks;
        } sphere;
        struct {
            float height;
            float radius;
            int slices;
            int stacks;
        } cone;
        struct {
            int length;
            int divisions;
        } plane;
        struct {
            int length;
            int divisions;
        } box;
    };
} *FIGURE;

std::vector<TRIANGLE>* get_triangles(FIGURE figure) {
    if (figure != nullptr) {
        return figure->triangles;
    }
    return nullptr;
}


FIGURE create_figure_empty() {
    FIGURE f = (FIGURE)malloc(sizeof(struct figure));
    f->type = UNKNOWN;
    f->triangles = new vector<TRIANGLE>();
    return f;
}

FIGURE create_figure_plane_box (FIGURE_TYPE type, int length, int divisions) {
    FIGURE f = create_figure_empty();
    f->type = type;

    switch (type) 
    {
        case PLANE:
            f->plane.length = length;
            f->plane.divisions = divisions;
            f->triangles = new vector<TRIANGLE>();
            break;
        case BOX:
            f->box.length = length;
            f->box.divisions = divisions;
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }
    return f;
}

FIGURE create_figure_sphere (float radius, int slices, int stacks) {
    FIGURE f = create_figure_empty();
    f->type = SPHERE;
    f->sphere.radius = radius;
    f->sphere.slices = slices;
    f->sphere.stacks = stacks;
    return f;
}

FIGURE create_figure_cone (float height, float radius, int slices, int stacks) {
    FIGURE f = create_figure_empty();
    f->type = CONE;
    f->cone.height = height;
    f->cone.radius = radius;
    f->cone.slices = slices;
    f->cone.stacks = stacks;
    return f;
}

void add_triangle(FIGURE f, TRIANGLE t) {
    if (f != NULL) {
        f->triangles->push_back(t);
    }
}

void add_triangles (FIGURE f, std::vector<TRIANGLE> t) {
    if (f != NULL) {
        f->triangles->insert(f->triangles->end(), t.begin(), t.end());
    }
}

int number_triangles(FIGURE f){
    if (f != NULL){
        return f->triangles->size();
    }
    return 0;
}



void save_file(FIGURE f, std::string filename) {
    
    const char* filename_cstr = filename.c_str();

    FILE* file = fopen(filename_cstr, "w"); 
    if (!file) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", filename_cstr);
        return;
    }

    switch (f->type) {
       case BOX:
            fprintf(file, "BOX\nLength: %d\nDivisions: %d\nNº de Triângulos: %d\n", f->box.length, f->box.divisions, number_triangles(f));
            fprintf(file, "\n%s", print_triangulos(f).c_str());
            break;
        case CONE:
            fprintf(file, "CONE\nHeight: %.2f\nRadius: %.2f\nSlices: %d\nStacks: %d\nNº de Triângulos: %d\n",
                    f->cone.height, f->cone.radius, f->cone.slices, f->cone.stacks, number_triangles(f));
            fprintf(file, "\n%s", print_triangulos(f).c_str());
            break;
        case PLANE:
            fprintf(file, "PLANE\nLength: %d\nDivisions: %d\nNº de Triângulos: %d\n", f->plane.length, f->plane.divisions, number_triangles(f));
            fprintf(file, "\n%s", print_triangulos(f).c_str()); 
            break;
        case SPHERE:
            fprintf(file, "SPHERE\nRadius: %.2f\nSlices: %d\nStacks: %d\nNº de Triângulos: %d\n",
                    f->sphere.radius, f->sphere.slices, f->sphere.stacks, number_triangles(f));
            fprintf(file, "\n%s", print_triangulos(f).c_str());
            break;
        default:
            fprintf(stderr, "Erro: Tipo desconhecido\n");
            break;
    }

    fclose(file);
    free(f);
}



FIGURE fileToFigure(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return nullptr;
    }

    std::string triangles = "TRIANGULOS:";
    FIGURE figure = create_figure_empty(); 
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "CONE") {
            float height, radius;
            int slices, stacks;
            iss >> height >> radius >> slices >> stacks >> triangles;
            figure = create_figure_cone(height, radius, slices, stacks);

            while (getline(file, line) && !line.empty() && line.find(triangles) == std::string::npos) {
                //ignorar o inicio
            }

            while (getline(file, line) && !line.empty()) {
                float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                sscanf(line.c_str(), "[(%f, %f, %f),(%f, %f, %f),(%f, %f, %f)]",
                       &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);

                POINT p1 = new_point(x1, y1, z1);
                POINT p2 = new_point(x2, y2, z2);
                POINT p3 = new_point(x3, y3, z3);

                TRIANGLE triangle = create_triangle();
                add_vertex(triangle, p1);
                add_vertex(triangle, p2);
                add_vertex(triangle, p3);
                add_triangle(figure, triangle);
            }
        } else if (type == "SPHERE") {
            float radius;
            int slices, stacks;
            iss >> radius >> slices >> stacks >> triangles;
            figure = create_figure_sphere(radius, slices, stacks);

            while (getline(file, line) && !line.empty() && line.find(triangles) == std::string::npos) {
                //ignorar o inicio
            }

            while (getline(file, line) && !line.empty()) {
                float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                sscanf(line.c_str(), "[(%f, %f, %f),(%f, %f, %f),(%f, %f, %f)]",
                       &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);
                POINT p1 = new_point(x1, y1, z1);
                POINT p2 = new_point(x2, y2, z2);
                POINT p3 = new_point(x3, y3, z3);

                TRIANGLE triangle = create_triangle();
                add_vertex(triangle, p1);
                add_vertex(triangle, p2);
                add_vertex(triangle, p3);
                add_triangle(figure, triangle);
            }
        } else if (type == "PLANE" || type == "BOX") {
            int length, divisions;
            iss >> length;
            if (type == "PLANE") {
                figure = create_figure_plane_box(PLANE, length, divisions);
            } else if (type == "BOX") {
                figure = create_figure_plane_box(BOX, length, divisions);
            }

            while (getline(file, line) && !line.empty() && line.find(triangles) == std::string::npos) {
                //ignorar o inicio
            }

            while (getline(file, line) && !line.empty()) {

                float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                sscanf(line.c_str(), "[(%f, %f, %f),(%f, %f, %f),(%f, %f, %f)]",
                       &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);

                POINT p1 = new_point(x1, y1, z1);
                POINT p2 = new_point(x2, y2, z2);
                POINT p3 = new_point(x3, y3, z3);

                TRIANGLE triangle = create_triangle();
                add_vertex(triangle, p1);
                add_vertex(triangle, p2);
                add_vertex(triangle, p3);
                add_triangle(figure, triangle);
            }
        }
    }
    //print_figura(figure);
    file.close();
    return figure;
}


void concat_FIGURES (FIGURE f1, FIGURE f2) {
    if (f2) {
        f1->triangles->insert(f1->triangles->end(), f2->triangles->begin(), f2->triangles->end());
    }
}

void print_figura(FIGURE f) {
    if (f != NULL) {
        if (f->type == PLANE) {
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
        if (f->type == BOX){
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
        if (f->type == SPHERE){
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
        if (f->type == CONE){
            printf("Vertices:\n");
            for (const TRIANGLE &triangulo : *(f->triangles)) {
                std::vector<POINT>* vertexBegin = get_points(triangulo);
                for (const POINT &vertex : *vertexBegin) {
                    printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
                }
            }
        }
    }
}


std::string print_triangulos(FIGURE f) {
    std::string output;
    if (!f) {
        output = "Error: Invalid figure.";
        return output;
    }

    std::vector<TRIANGLE>* triangles = f->triangles;

    if (!triangles) {
        output = "Error: No triangles in the figure.";
        return output;
    }

    for (const TRIANGLE &triangulo : *triangles) {
        std::vector<POINT>* vertices = get_points(triangulo);
        if (!vertices || vertices->size() != 3) {
            output += "Error: Invalid triangle - should have exactly 3 vertices.\n";
            continue;
        }
        output += "[";
        for (size_t j = 0; j < vertices->size(); ++j) {
            const POINT& point = (*vertices)[j];
            char buffer[100]; 
            snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)", get_X(point), get_Y(point), get_Z(point));
            output += buffer;
            if (j < vertices->size() - 1) output += ",";
        }
        output += "]\n";
    }

    return output;
}

void free_figure(FIGURE f) {
    if (f != NULL) {
        if (f->triangles != NULL) {
            delete f->triangles;
        }
        free(f);
    }
}