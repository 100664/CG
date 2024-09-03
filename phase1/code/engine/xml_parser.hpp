// xml_parser.hpp
#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include "../tinyxml/tinyxml.h"

struct Camera {
    float position[3];
    float lookAt[3];
    float up[3];
    struct Projection {
        float fov;
        float near;
        float far;
    } projection;
};

struct Model {
    std::string file;
};

struct Group {
    std::vector<Model> models;
};

struct WORLD {
    int windowWidth;
    int windowHeight;
    Camera camera;
    Group group;
};

WORLD create_world ();

Camera create_new_camera();

Model create_new_model();

Group create_new_group();

float get_position_camX(WORLD w);

void set_position_camX(WORLD w, float x);

float get_position_camY(WORLD w);

void set_position_camY(WORLD w, float y);

float get_position_camZ(WORLD w);

void set_position_camZ(WORLD w, float z);

float get_lookAt_camX(WORLD w);

void set_lookAt_camX(WORLD w, float x);

float get_lookAt_camY(WORLD w);

void set_lookAt_camY(WORLD w, float y);

float get_lookAt_camZ(WORLD w);

void set_lookAt_camZ(WORLD w, float z);

float get_up_camX(WORLD w);

void set_up_camX(WORLD w, float x);

float get_up_camY(WORLD w);

void set_up_camY(WORLD w, float y);

float get_up_camZ(WORLD w);

void set_up_camZ(WORLD w, float z);

float get_fov(WORLD w);

float get_near(WORLD w);

float get_far(WORLD w);

std::vector<Model> get_models(WORLD &w);

void parse_config_file(const char* filename, WORLD& world) ;

void delete_world(WORLD &w);

#endif // XML_PARSER_H