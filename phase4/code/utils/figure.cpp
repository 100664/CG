#include "figure.hpp"


typedef struct figure {
    FIGURE_TYPE type;
    std::vector<TRIANGLE>* triangles;
    std::vector<POINT>* normals;
    std::vector<POINT>* textures;
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
        struct {
            float inner_radius;
            float outer_radius;
            int slices;
        } ring;
        struct {
            float tessellation;
            const char* patches_file;
        };   
    };
} *FIGURE;

std::vector<TRIANGLE>* get_triangles(FIGURE figure) {
    if (figure != nullptr) {
        return figure->triangles;
    }
    return nullptr;
}

std::vector<POINT>* get_normals(FIGURE figure) {
    return figure->normals;
}

std::vector<POINT>* get_textures(FIGURE figure) {
    return figure->textures;
}


FIGURE create_figure_empty() {
    FIGURE f = (FIGURE)malloc(sizeof(struct figure));
    f->type = UNKNOWN;
    f->triangles = new std::vector<TRIANGLE>();
    f->normals = new std::vector<POINT>();
    f->textures = new std::vector<POINT>();
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
            f->triangles = new std::vector<TRIANGLE>();
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

FIGURE create_figure_ring(float inner_radius, float outer_radius, int slices) {
    FIGURE f = create_figure_empty();
    f->type = RING;
    f->ring.inner_radius = inner_radius;
    f->ring.outer_radius = outer_radius;
    f->ring.slices = slices;
    return f;
}


FIGURE create_figure_bezier(float tessellation, const char* patches_file) {
    FIGURE f = create_figure_empty();
    f->type = BEZIER;
    f->tessellation = tessellation;
    f->patches_file = patches_file;
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
            fprintf(file, "BOX\nLength: %d\nDivisions: %d\nTriangulos: %d\n", f->box.length, f->box.divisions, number_triangles(f));
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;
        case CONE:
            fprintf(file, "CONE\nHeight: %.2f\nRadius: %.2f\nSlices: %d\nStacks: %d\nTriangulos: %d\n",
                    f->cone.height, f->cone.radius, f->cone.slices, f->cone.stacks, number_triangles(f));
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;
        case PLANE:
            fprintf(file, "PLANE\nLength: %d\nDivisions: %d\nTriangulos: %d\n", f->plane.length, f->plane.divisions, number_triangles(f));
            fprintf(file, "%s", print_triangulos(f).c_str()); 
            break;
        case SPHERE:
            fprintf(file, "SPHERE\nRadius: %.2f\nSlices: %d\nStacks: %d\nTriangulos: %d\n",
                    f->sphere.radius, f->sphere.slices, f->sphere.stacks, number_triangles(f));
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;
        case RING: 
            fprintf(file, "RING\nInner Radius: %.2f\nOuter Radius: %.2f\nSlices: %d\nTriangulos: %d\n",
                    f->ring.inner_radius, f->ring.outer_radius, f->ring.slices, number_triangles(f));
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;   
        case BEZIER:
            fprintf(file, "BEZIER\nTessellation: %.2f\nPatches File: %s\nTriangulos: %d\n",
                    f->tessellation, f->patches_file, number_triangles(f));
            fprintf(file, "%s", print_triangulos(f).c_str());
            break;   
        default:
            fprintf(stderr, "Erro: Impossível guardar figura no ficheiro\n");
            break;
    }

    fclose(file);
    free(f);
}


FIGURE fileToFigure(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo. figure.cpp" << std::endl;
        return nullptr;
    }

    FIGURE figure = create_figure_empty();
    std::string line;

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "CONE") {
            float height = 0.0f, radius = 0.0f;
            int slices = 0, stacks = 0, triangles = 0;

            std::getline(file, line); sscanf(line.c_str(), "Height: %f", &height);
            std::getline(file, line); sscanf(line.c_str(), "Radius: %f", &radius);
            std::getline(file, line); sscanf(line.c_str(), "Slices: %d", &slices);
            std::getline(file, line); sscanf(line.c_str(), "Stacks: %d", &stacks);
            std::getline(file, line); sscanf(line.c_str(), "Triangulos: %d", &triangles);

            figure = create_figure_cone(height, radius, slices, stacks);
        } else if (type == "SPHERE") {
            float radius = 0.0f;
            int slices = 0, stacks = 0, triangles = 0;

            std::getline(file, line); sscanf(line.c_str(), "Radius: %f", &radius);
            std::getline(file, line); sscanf(line.c_str(), "Slices: %d", &slices);
            std::getline(file, line); sscanf(line.c_str(), "Stacks: %d", &stacks);
            std::getline(file, line); sscanf(line.c_str(), "Triangulos: %d", &triangles);

            figure = create_figure_sphere(radius, slices, stacks);
        } else if (type == "PLANE") {
            int length = 0, divisions = 0, triangles = 0;

            std::getline(file, line); sscanf(line.c_str(), "Length: %d", &length);
            std::getline(file, line); sscanf(line.c_str(), "Divisions: %d", &divisions);
            std::getline(file, line); sscanf(line.c_str(), "Triangulos: %d", &triangles);

            figure = create_figure_plane_box(PLANE, length, divisions);
        } else if (type == "BOX") {
            int length = 0, divisions = 0, triangles = 0;

            std::getline(file, line); sscanf(line.c_str(), "Length: %d", &length);
            std::getline(file, line); sscanf(line.c_str(), "Divisions: %d", &divisions);
            std::getline(file, line); sscanf(line.c_str(), "Triangulos: %d", &triangles);

            figure = create_figure_plane_box(BOX, length, divisions);
        } else if (type == "RING") {
            float inner_radius = 0.0f, outer_radius = 0.0f;
            int slices = 0, triangles = 0;

            std::getline(file, line); sscanf(line.c_str(), "InnerRadius: %f", &inner_radius);
            std::getline(file, line); sscanf(line.c_str(), "OuterRadius: %f", &outer_radius);
            std::getline(file, line); sscanf(line.c_str(), "Slices: %d", &slices);
            std::getline(file, line); sscanf(line.c_str(), "Triangulos: %d", &triangles);

            figure = create_figure_ring(inner_radius, outer_radius, slices);
        } else if (type == "BEZIER") {
            float tessellation = 0.0f;
            char * patches_file = nullptr;
            int triangles = 0;

            std::getline(file, line); sscanf(line.c_str(), "Tessellation: %f", &tessellation);
            std::getline(file, line); std::istringstream patches_file_iss(line); patches_file_iss >> patches_file;
            std::getline(file, line); sscanf(line.c_str(), "Triangulos: %d", &triangles);

            figure = create_figure_bezier(tessellation, patches_file);
        }

        while (getline(file, line) && !line.empty()) {
            if (line == "VERTICES:") {
                while (getline(file, line) && !line.empty() && line[0] == '[') {
                    float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                    sscanf(line.c_str(), "[(%f, %f, %f)(%f, %f, %f)(%f, %f, %f)]",
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
            } else if (line == "NORMAIS:") {
                while (getline(file, line) && !line.empty() && line[0] == '(') {
                    float x, y, z;
                    sscanf(line.c_str(), "(%f, %f, %f)", &x, &y, &z);
                    POINT normal = new_point(x, y, z);
                    add_normal(figure, normal);
                }
            } else if (line == "TEXTURAS:") {
                while (getline(file, line) && !line.empty() && line[0] == '(') {
                    float x, y, z;
                    sscanf(line.c_str(), "(%f, %f, %f)", &x, &y, &z);
                    POINT texture = new_point(x, y, z);
                    add_texture(figure, texture);
                }
            } else {
                printf("LINHA INVALIDA: %s\n", line.c_str());
            }

        }
            //print_figura_vertices(figure);
            //printf_normais(figure); 
            //printf_texturas(figure);
    }

    file.close();
    return figure;
}



void concat_FIGURES (FIGURE f1, FIGURE f2) {
    if (f2) {
        f1->triangles->insert(f1->triangles->end(), f2->triangles->begin(), f2->triangles->end());
        f1->normals->insert(f1->normals->end(), f2->normals->begin(), f2->normals->end());
        f1->textures->insert(f1->textures->end(), f2->textures->begin(), f2->textures->end());
    }
}

void print_figura_vertices(FIGURE f) {
    if (f != NULL) {
        std::vector<TRIANGLE>* triangles = f->triangles;

        if (!triangles) {
            printf("Error: No triangles in the figure.\n");
            return;
        }

        printf("Vertices:\n");
        for (const TRIANGLE &triangulo : *triangles) {
            std::vector<POINT>* vertexBegin = get_points(triangulo);
            for (size_t j = 0; j < vertexBegin->size(); ++j) {
                const POINT &vertex = (*vertexBegin)[j];
                printf("(%.2f, %.2f, %.2f)\n", get_X(vertex), get_Y(vertex), get_Z(vertex));
            }
        }
    }
}

void printf_normais (FIGURE f){
    std::vector<POINT>* normais = get_normals(f);
    for (const POINT& normal: *normais) {
        printf("Normais(x: %f, y: %f, z:%f)\n", get_X(normal), get_Y(normal), get_Z(normal));
    }
}


void printf_texturas (FIGURE f){
    std::vector<POINT>* textures = get_textures(f);
    for (const POINT& textura: *textures) {
        printf("Texturas(x: %f, y: %f, z:%f)\n", get_X(textura), get_Y(textura), get_Z(textura));
    }
}

std::string print_triangulos(FIGURE f) {
    std::string output;
    if (!f) {
        output = "Error: Invalid figure.";
        return output;
    }

    std::vector<TRIANGLE>* triangles = f->triangles;
    std::vector<POINT>* normals = get_normals(f);
    std::vector<POINT>* textures = get_textures(f);

    if (!triangles) {
        output = "Error: No triangles in the figure.";
        return output;
    }

    output += "VERTICES:\n";
    for (const TRIANGLE &triangulo : *triangles) {
        std::vector<POINT>* vertices = get_points(triangulo);
        if (!vertices || vertices->size() != 3) {
            output += "Error: Invalid triangle - should have exactly 3 vertices.\n";
            continue;
        }
        output += "[";
        for (size_t j = 0; j < vertices->size(); ++j) {
            const POINT& point = (*vertices)[j];
            char buffer[100]; // Buffer to store point coordinates
            snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)", get_X(point), get_Y(point), get_Z(point));
            output += buffer;
        }
        output += "]\n";

    }

    output += "\nNORMAIS:\n";

    for (size_t j = 0; j < normals->size(); ++j) 
    {
        const POINT& normal = (*normals)[j];
        char buffer[100]; // Buffer to store point coordinates
        snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)\n", get_X(normal), get_Y(normal), get_Z(normal));
        output += buffer;
    }
    output += "\nTEXTURAS:\n";

    for (size_t j = 0; j < textures->size(); ++j) 
    {
        const POINT& texture = (*textures)[j];
        char buffer[100]; // Buffer to store point coordinates
        snprintf(buffer, sizeof(buffer), "(%.2f, %.2f, %.2f)\n", get_X(texture), get_Y(texture), get_Z(texture));
        output += buffer;
    }
    output += "\n";

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

std::vector<float> figure_to_vectors(const FIGURE& figure) {
    std::vector<float> vectors;
    if (figure->triangles) {
        vectors.reserve(figure->triangles->size() * 3 * 3); // Cada triângulo tem 3 vértices, cada vértice tem 3 coordenadas

        for (const TRIANGLE& triangle : *(figure->triangles)) 
        {
            const std::vector<POINT>& vertices = *get_points(triangle);
            for (const POINT& vertex : vertices) 
            {
                vectors.push_back(get_X(vertex));
                vectors.push_back(get_Y(vertex));
                vectors.push_back(get_Z(vertex));
            }
        }
    }
    return vectors;
}

void add_normal(FIGURE f, POINT p) {
    if (f != nullptr) {
        f->normals->push_back(p);
    }
}

void add_texture(FIGURE f, POINT p) {
    if (f != nullptr) {
        f->textures->push_back(p);
    }
}

void add_normals(FIGURE f, const std::vector<POINT>& normals) {
    if (f != nullptr) {
        for (const POINT& normal : normals) {
            add_normal(f, normal);
        }
    }
}

void add_textures(FIGURE f, const std::vector<POINT>& textures) {
    if (f != nullptr) {
        for (const POINT& texture : textures) {
            add_texture(f, texture);
        }
    }
}


std::vector<float> figure_to_normals(FIGURE f){
    std::vector<float> normals;
    for (const POINT &normal : *(f->normals)) {
        normals.push_back(get_X(normal));
        normals.push_back(get_Y(normal));
        normals.push_back(get_Z(normal));
    }
    return normals;
}

std::vector<float> figure_to_textures(FIGURE f){
    std::vector<float> textures;
    for (const POINT &texture : *(f->textures)) {
        textures.push_back(get_X(texture));
        textures.push_back(get_Y(texture));
        textures.push_back(get_Z(texture));
    }
    return textures;
}