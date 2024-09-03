#include "bezier.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Function to parse the patch file
std::vector<std::vector<POINT> > parse_patch_file(const char* filePath) {
    std::ifstream file(filePath);
    std::vector<std::vector<POINT> > result;

    if (file.is_open()) {
        std::string line;
        if (!std::getline(file, line)) return result;
        int numPatches = std::stoi(line);
        std::vector<std::vector<int> > indicesPerPatch;

        for (int i = 0; i < numPatches; i++) {
            if (!std::getline(file, line)) return result;
            std::istringstream iss(line);
            std::vector<int> indices;
            int index;
            while (iss >> index) {
                indices.push_back(index);
                if (iss.peek() == ',') iss.ignore();
            }
            indicesPerPatch.push_back(indices);
        }

        if (!std::getline(file, line)) return result;
        int numControlPoints = std::stoi(line);

        std::vector<POINT> controlPoints;
        for (int i = 0; i < numControlPoints; i++) {
            if (!std::getline(file, line)) return result;
            std::istringstream iss(line);
            float x, y, z;
            char comma;
            if (!(iss >> x >> comma >> y >> comma >> z))
                return result;
            POINT point = new_point(x, y, z);
            controlPoints.push_back(point);
        }

        for (const std::vector<int>& indices : indicesPerPatch) {
            std::vector<POINT> patch;
            for (int indice : indices) {
                patch.push_back(controlPoints[indice]);
            }
            result.push_back(patch);
        }
        file.close();
    }
    printf("Parsed %lu patches\n", result.size());
    return result;
}

// Function to interpolate a row of control points at parameter t
POINT bezier_interpolate_row(const std::vector<POINT>& controlPoints, float t) {
    std::vector<float> basis(4);
    basis[0] = -1.0f * t * t * t + 3.0f * t * t - 3.0f * t + 1.0f;
    basis[1] = 3.0f * t * t * t - 6.0f * t * t + 3.0f * t;
    basis[2] = -3.0f * t * t * t + 3.0f * t * t;
    basis[3] = 1.0f * t * t * t;

    float x = 0.0f, y = 0.0f, z = 0.0f;
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        x += basis[i] * get_X(controlPoints[i]);
        y += basis[i] * get_Y(controlPoints[i]);
        z += basis[i] * get_Z(controlPoints[i]);
    }

    return new_point(x, y, z);
}

// Function to interpolate a point on the Bezier patch
POINT bezier_interpolate(const std::vector<POINT>& patch, float u, float v) {
    std::vector<POINT> points;
    for (int i = 0; i < 4; i++) {
        std::vector<POINT> row;
        for (int j = 0; j < 4; j++) {
            row.push_back(patch[i * 4 + j]);
        }
        points.push_back(bezier_interpolate_row(row, u));
    }
    return bezier_interpolate_row(points, v);
}

// Function to compute the derivative of a row of control points at parameter t
POINT bezier_derivative_row(const std::vector<POINT>& controlPoints, float t) {
    std::vector<float> basis(4);
    basis[0] = -3.0f * t * t + 6.0f * t - 3.0f;
    basis[1] = 9.0f * t * t - 12.0f * t + 3.0f;
    basis[2] = -9.0f * t * t + 6.0f * t;
    basis[3] = 3.0f * t * t;

    float x = 0.0f, y = 0.0f, z = 0.0f;
    for (size_t i = 0; i < controlPoints.size(); ++i) {
        x += basis[i] * get_X(controlPoints[i]);
        y += basis[i] * get_Y(controlPoints[i]);
        z += basis[i] * get_Z(controlPoints[i]);
    }

    return new_point(x, y, z);
}

// Function to compute the partial derivative with respect to u
POINT bezier_derivative_u(const std::vector<POINT>& patch, float u, float v) {
    std::vector<POINT> points;
    for (int i = 0; i < 4; i++) {
        std::vector<POINT> row;
        for (int j = 0; j < 4; j++) {
            row.push_back(patch[i * 4 + j]);
        }
        points.push_back(bezier_derivative_row(row, u));
    }
    return bezier_interpolate_row(points, v);
}

// Function to compute the partial derivative with respect to v
POINT bezier_derivative_v(const std::vector<POINT>& patch, float u, float v) {
    std::vector<POINT> points;
    for (int i = 0; i < 4; i++) {
        std::vector<POINT> row;
        for (int j = 0; j < 4; j++) {
            row.push_back(patch[i * 4 + j]);
        }
        points.push_back(bezier_interpolate_row(row, u));
    }
    return bezier_derivative_row(points, v);
}

// Function to generate the Bezier surface
std::vector<TRIANGLE> generate_bezier_surfaces(FIGURE& f, const std::vector<POINT>& patch, float tessellation) {
    std::vector<TRIANGLE> triangles;
    std::vector<POINT> vertices;

    for (int i = 0; i < tessellation; i++) {
        for (int j = 0; j < tessellation; j++) {
            float u = static_cast<float>(i) / tessellation;
            float v = static_cast<float>(j) / tessellation;

            POINT p1 = bezier_interpolate(patch, u, v);
            POINT p2 = bezier_interpolate(patch, u + 1.0f / tessellation, v);
            POINT p3 = bezier_interpolate(patch, u, v + 1.0f / tessellation);
            POINT p4 = bezier_interpolate(patch, u + 1.0f / tessellation, v + 1.0f / tessellation);

            POINT du1 = bezier_derivative_u(patch, u, v);
            POINT dv1 = bezier_derivative_v(patch, u, v);
            POINT normal1;
            cross(du1, dv1, normal1);
            safe_normalize(normal1);

            POINT du2 = bezier_derivative_u(patch, u + 1.0f / tessellation, v);
            POINT dv2 = bezier_derivative_v(patch, u + 1.0f / tessellation, v);
            POINT normal2;
            cross(du2, dv2, normal2);
            safe_normalize(normal2);

            POINT du3 = bezier_derivative_u(patch, u, v + 1.0f / tessellation);
            POINT dv3 = bezier_derivative_v(patch, u, v + 1.0f / tessellation);
            POINT normal3;
            cross(du3, dv3, normal3);
            safe_normalize(normal3);

            POINT du4 = bezier_derivative_u(patch, u + 1.0f / tessellation, v + 1.0f / tessellation);
            POINT dv4 = bezier_derivative_v(patch, u + 1.0f / tessellation, v + 1.0f / tessellation);
            POINT normal4;
            cross(du4, dv4, normal4);
            safe_normalize(normal4);

            vertices.push_back(p1);
            vertices.push_back(p2);
            vertices.push_back(p3);
            vertices.push_back(p4);

            add_normal(f, normal1);
            add_normal(f, normal2);
            add_normal(f, normal3);
            add_normal(f, normal4);

            TRIANGLE t1 = create_triangle();
            add_vertex(t1, p1);
            add_vertex(t1, p2);
            add_vertex(t1, p3);

            TRIANGLE t2 = create_triangle();
            add_vertex(t2, p2);
            add_vertex(t2, p3);
            add_vertex(t2, p4);

            triangles.push_back(t1);
            triangles.push_back(t2);

            //NAO ESTAO BEM - OBVIAMENTE :)
            add_texture(f, normal1);
            add_texture(f, normal1);
            add_texture(f, normal1);
            add_texture(f, normal1);
        }
    }

    return triangles;
}

// Function to generate a figure from Bezier patches
FIGURE generate_bezier(float tessellation, const char* patches_file) {
    FIGURE figure = create_figure_bezier(tessellation, patches_file);
    std::vector<std::vector<POINT> > patches = parse_patch_file(patches_file);

    for (const std::vector<POINT>& patch : patches) {
        std::vector<TRIANGLE> triangles = generate_bezier_surfaces(figure, patch, tessellation);

        // Add triangles to the figure
        for (const TRIANGLE& t : triangles) {
            add_triangle(figure, t);
        }
    }

    return figure;
}