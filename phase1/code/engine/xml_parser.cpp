#define GL_SILENCE_DEPRECATION
#include "xml_parser.hpp"


Camera create_new_camera() {
    Camera c;
    for (int i = 0; i < 3; ++i) {
        c.position[i] = 0.0f;
        c.lookAt[i] = 0.0f;
        c.up[i] = 0.0f;
    }
    c.projection.fov = 0.0f;
    c.projection.near = 0.0f;
    c.projection.far = 0.0f;
    return c;
}

Model create_new_model() {
    Model m;
    m.file = "";
    return m;
}

Group create_new_group(){
    Group g;
    g.models = std::vector<Model>();
    return g;
}

WORLD create_world (){
    WORLD w;
    w.windowWidth = 0;
    w.windowHeight = 0;
    w.camera = create_new_camera();
    w.group = create_new_group();
    return w;
}

float get_position_camX(WORLD w) {
    return w.camera.position[0];
}

void set_position_camX(WORLD w, float x) {
    w.camera.position[0] = x;
}

float get_position_camY(WORLD w) {
    return w.camera.position[1];
}

void set_position_camY(WORLD w, float y) {
    w.camera.position[1] = y;
}

float get_position_camZ(WORLD w) {
    return w.camera.position[2];
}

void set_position_camZ(WORLD w, float z) {
    w.camera.position[2] = z;
}

float get_lookAt_camX(WORLD w) {
    return w.camera.lookAt[0];
}

void set_lookAt_camX(WORLD w, float x) {
    w.camera.lookAt[0] = x;
}

float get_lookAt_camY(WORLD w) {
    return w.camera.lookAt[1];
}

void set_lookAt_camY(WORLD w, float y) {
    w.camera.lookAt[1] = y;
}

float get_lookAt_camZ(WORLD w) {
    return w.camera.lookAt[2];
}

void set_lookAt_camZ(WORLD w, float z) {
    w.camera.lookAt[2] = z;
}

float get_up_camX(WORLD w) {
    return w.camera.up[0];
}

void set_up_camX(WORLD w, float x) {
    w.camera.up[0] = x;
}

float get_up_camY(WORLD w) {
    return w.camera.up[1];
}

void set_up_camY(WORLD w, float y) {
    w.camera.up[1] = y;
}

float get_up_camZ(WORLD w) {
    return w.camera.up[2];
}

void set_up_camZ(WORLD w, float z) {
    w.camera.up[2] = z;
}

float get_fov(WORLD w) {
    return w.camera.projection.fov;
}

float get_near(WORLD w) {
    return w.camera.projection.near;
}

float get_far(WORLD w) {
    return w.camera.projection.far;
}

std::vector<Model> get_models(WORLD &w) {
    return w.group.models;
}

void parse_config_file(const char* filename, WORLD& world) {
    TiXmlDocument doc(filename);
    if (!doc.LoadFile()) {
        std::cerr << "Error loading XML file: " << doc.ErrorDesc() << std::endl;
        return;
    }

    TiXmlElement* worldElement = doc.FirstChildElement("world");

    if (!worldElement) {
        std::cerr << "Error: Missing 'world' element in the XML file." << std::endl;
        return;
    }

    // Example: Parse window settings
    TiXmlElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        windowElement->QueryIntAttribute("width", &world.windowWidth);
        windowElement->QueryIntAttribute("height", &world.windowHeight);
    }

    // Example: Parse camera settings
    TiXmlElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        // Parse camera attributes
        TiXmlElement* positionElement = cameraElement->FirstChildElement("position");
        if (positionElement) {
            positionElement->QueryFloatAttribute("x", &world.camera.position[0]);
            positionElement->QueryFloatAttribute("y", &world.camera.position[1]);
            positionElement->QueryFloatAttribute("z", &world.camera.position[2]);
        }

        TiXmlElement* lookAtElement = cameraElement->FirstChildElement("lookAt");
        if (lookAtElement) {
            lookAtElement->QueryFloatAttribute("x", &world.camera.lookAt[0]);
            lookAtElement->QueryFloatAttribute("y", &world.camera.lookAt[1]);
            lookAtElement->QueryFloatAttribute("z", &world.camera.lookAt[2]);
        }

        TiXmlElement* upElement = cameraElement->FirstChildElement("up");
        if (upElement) {
            upElement->QueryFloatAttribute("x", &world.camera.up[0]);
            upElement->QueryFloatAttribute("y", &world.camera.up[1]);
            upElement->QueryFloatAttribute("z", &world.camera.up[2]);
        }

        TiXmlElement* projectionElement = cameraElement->FirstChildElement("projection");
        if (projectionElement) {
            projectionElement->QueryFloatAttribute("fov", &world.camera.projection.fov);
            projectionElement->QueryFloatAttribute("near", &world.camera.projection.near);
            projectionElement->QueryFloatAttribute("far", &world.camera.projection.far);
        }
    }

    // Example: Parse models
    TiXmlElement* modelsElement = worldElement->FirstChildElement("group")->FirstChildElement("models");
    if (modelsElement) {
        for (TiXmlElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
            Model model;
            const char* fileAttribute = modelElement->Attribute("file"); // Corrected line
            if (fileAttribute) {
                model.file = fileAttribute;
                world.group.models.push_back(model);
            } else {
                std::cerr << "Error: Model element is missing 'file' attribute." << std::endl;
            }
        }
    }
}

void delete_world(WORLD &w) {
    w.windowWidth = 0;
    w.windowHeight = 0;
    for (int i = 0; i < 3; ++i) {
        w.camera.position[i] = 0.0f;
        w.camera.lookAt[i] = 0.0f;
        w.camera.up[i] = 0.0f;
    }
    w.camera.projection.fov = 0.0f;
    w.camera.projection.near = 0.0f;
    w.camera.projection.far = 0.0f;

    /*for (Model& model : w.group.models)
        model.file.clear();*/
    w.group.models.clear();
}