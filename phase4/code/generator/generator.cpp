#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <fstream>
#include <cmath>
#include "plane.hpp"
#include "../generator/box.hpp"
#include "../generator/cone.hpp"
#include "../generator/sphere.hpp"
#include "../generator/ring.hpp"
#include "../generator/bezier.hpp"
#include "../utils/figure.hpp"
#include "../utils/point.hpp"
#include "../utils/triangle.hpp"

int main(int argc, char* argv[]) {
    // Check the number of command-line arguments
    if (argc < 5) {
        std::cerr << "Usage: generator <primitive_type> <params...> <output_file>" << std::endl;
        return 1;
    }

    // Extract command-line arguments
    std::string primitive_type = argv[1];
    std::string output_file = argv[argc - 1];

    // Create a figure object
    FIGURE figure = create_figure_empty();

    // Check the primitive type and generate the corresponding shape
    if (primitive_type == "plane" && argc == 5) {
        int length = std::stoi(argv[2]);
        int divisions = std::stoi(argv[3]);
        figure = generate_plane_XZ(length, divisions, 0.0, 0);
        //print_figura(figure);
    } else if (primitive_type == "box" && argc == 5) {
        float length = std::stof(argv[2]);
        int divisions = std::stoi(argv[3]);
        figure = generate_box(length, divisions);
    }else if (primitive_type == "sphere" && argc == 6) {
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        figure = generate_sphere(radius, slices, stacks);

        //print_figura(figure);

    }else if (primitive_type == "cone" && argc == 7) {
        float bottom_radius = std::stof(argv[2]);
        float height = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        int stacks = std::stoi(argv[5]);
        figure = generate_cone(bottom_radius, height, slices, stacks);

    }else if (primitive_type == "ring" && argc == 6){
        float inner_radius = std::stof(argv[2]);
        float outer_radius = std::stof(argv[3]);
        int slices = std::stoi(argv[4]);
        figure = generate_ring(inner_radius, outer_radius, slices);
        
        //print_figura(figure);
    }
    else if(primitive_type == "bezier" && argc == 5){
            float tessellation = atoi(argv[2]);
            const char* patches_file = argv[3];
            figure = generate_bezier(tessellation, patches_file);
            //print_figura(figure);
    }
    else {
        std::cerr << "Invalid arguments or primitive type." << std::endl;
        return 1;
   } 
    save_file(figure, output_file);
    //free_figure(figure); não se pode dar free, pois o std já faz isso então estavamos a dar double free

    return 0;
}
