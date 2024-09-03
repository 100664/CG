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

MODEL create_new_model() {
    Model m;
    m.file = "";
    return m;
}

GROUP create_new_group() {
    Group g;
    g.models = std::vector<Model>();
    g.transforms = std::vector<Transform>();
    return g;
}

WORLD create_world() {
    WORLD w;
    w.windowWidth = 0;
    w.windowHeight = 0;
    w.camera = create_new_camera();
    w.groups = std::vector<GROUP>();
    return w;
}

void parse_group_element(TiXmlElement* groupElement, Group& group) {
    // Parse models
    TiXmlElement* modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        for (TiXmlElement* modelElement = modelsElement->FirstChildElement("model"); modelElement; modelElement = modelElement->NextSiblingElement("model")) {
            Model model;
            const char* fileAttribute = modelElement->Attribute("file");
            if (fileAttribute) {
                model.file = fileAttribute;
                group.models.push_back(model);
            } else {
                std::cerr << "Error: Model element is missing 'file' attribute." << std::endl;
            }
        }
    }

    // Parse transforms
    for (TiXmlElement* transformElement = groupElement->FirstChildElement("transform"); transformElement; transformElement = transformElement->NextSiblingElement("transform")) {
        Transform transform;

        for (TiXmlElement* childElement = transformElement->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement()) {
            const char* transformType = childElement->Value();
            if (strcmp(transformType, "translate") == 0) {
                parse_translate_transform(childElement, transform);
                group.transforms.push_back(transform);
            } else if (strcmp(transformType, "rotate") == 0) {
                parse_rotate_transform(childElement, transform);
                group.transforms.push_back(transform);
            } else if (strcmp(transformType, "scale") == 0) {
                parse_scale_transform(childElement, transform);
                group.transforms.push_back(transform);
                
            } else {
                std::cerr << "Error: Unknown transform type '" << transformType << "'." << std::endl;
            }
        }
    }

    // Parse child groups
    for (TiXmlElement* childGroupElement = groupElement->FirstChildElement("group"); childGroupElement; childGroupElement = childGroupElement->NextSiblingElement("group")) {
        Group childGroup;
        parse_group_element(childGroupElement, childGroup);
        group.children.push_back(childGroup);
    }
}

void parse_translate_transform(TiXmlElement* translateElement, Transform& transform) {
    transform.type = TRANSLATE;
    translateElement->QueryFloatAttribute("x", &transform.translate.x);
    translateElement->QueryFloatAttribute("y", &transform.translate.y);
    translateElement->QueryFloatAttribute("z", &transform.translate.z);
    const char* timeAttribute = translateElement->Attribute("time");
    transform.time = (timeAttribute) ? std::atof(timeAttribute) : 0.0f;
    transform.align = false;

    const char* alignAttribute = translateElement->Attribute("align");
    if (alignAttribute && strcmp(alignAttribute, "true") == 0) {
        transform.align = true;
        for (TiXmlElement* pointElement = translateElement->FirstChildElement("point"); pointElement; pointElement = pointElement->NextSiblingElement("point")) {
            float x, y, z;
            if (pointElement->QueryFloatAttribute("x", &x) == TIXML_SUCCESS &&
                pointElement->QueryFloatAttribute("y", &y) == TIXML_SUCCESS &&
                pointElement->QueryFloatAttribute("z", &z) == TIXML_SUCCESS) {
                POINT point = new_point(x, y, z);
                transform.points.push_back(point);
            } else {
                std::cerr << "Error: Invalid point element in translate transform." << std::endl;
            }
        }
    }

    transform.y_aux = new_point(0.0f, 1.0f, 0.0f);
}

void parse_rotate_transform(TiXmlElement* rotateElement, Transform& transform) {
    transform.type = ROTATE;
    const char* timeAttribute = rotateElement->Attribute("time");
    transform.time = (timeAttribute) ? std::atof(timeAttribute) : 0.0f;
    rotateElement->QueryFloatAttribute("angle", &transform.rotate.angle);
    rotateElement->QueryFloatAttribute("x", &transform.rotate.x);
    rotateElement->QueryFloatAttribute("y", &transform.rotate.y);
    rotateElement->QueryFloatAttribute("z", &transform.rotate.z);
}

void parse_scale_transform(TiXmlElement* scaleElement, Transform& transform) {
    transform.type = SCALE;
    scaleElement->QueryFloatAttribute("x", &transform.scale.x);
    scaleElement->QueryFloatAttribute("y", &transform.scale.y);
    scaleElement->QueryFloatAttribute("z", &transform.scale.z);
}


void parse_config_file(char* filename, WORLD& world) {
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

    // Parse window settings
    TiXmlElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        windowElement->Attribute("width", &world.windowWidth);
        windowElement->Attribute("height", &world.windowHeight);
    }

    // Parse camera settings
    TiXmlElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
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

    // Clear existing groups
    world.groups.clear();
    // Parse groups and their transformations
    for (TiXmlElement* groupElement = worldElement->FirstChildElement("group"); groupElement; groupElement = groupElement->NextSiblingElement("group")) {
        Group group = create_new_group();
    
        parse_group_element(groupElement, group);
        world.groups.push_back(group);
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
    for (Group &group : w.groups) {
        group.models.clear(); // Limpar os modelos do grupo
        group.transforms.clear(); // Limpar as transformações do grupo
    }
    w.groups.clear();
}

int get_windowWidth(WORLD w) {
    return w.windowWidth;
}

int get_windowHeight(WORLD w) {
    return w.windowHeight;
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

float get_translate_X (TRANSFORM t) {
    return  t.translate.x;
}

float  get_translate_Y (TRANSFORM t) {
    return  t.translate.y;
}

float get_translate_Z (TRANSFORM t) {
    return t.translate.z;
}

float get_rotate_angle(TRANSFORM t) {
    return t.rotate.angle;
}

float get_rotate_X (TRANSFORM t) {
    return t.rotate.x;
}

float get_rotate_Y (TRANSFORM t) {
    return t.rotate.y;
}

float get_rotate_Z (TRANSFORM t){
    return t.rotate.z;
}

float get_scale_X (TRANSFORM t) {
    return t.scale.x;
}

float get_scale_Y (TRANSFORM t) {
    return t.scale.y;
}

float get_scale_Z (TRANSFORM t) {
    return t.scale.z;
}

int get_time (TRANSFORM t) {
    return t.time;
}

bool get_align (TRANSFORM t) {
    return t.align;
}

std::vector<MODEL> get_models_group(GROUP g) {
    std::vector<MODEL> models;
    for(const auto &model : g.models)
        models.push_back(model);
    
    for(const auto &childGroup : g.children)
    {    
        std::vector<MODEL> childModels = get_models_group(childGroup);
        models.insert(models.end(), childModels.begin(), childModels.end());
    }
    
    return models;
}

std::vector<MODEL> get_models(WORLD &w) {
    std::vector<MODEL> models;
    for (const auto &group : w.groups) 
    {
        std::vector<MODEL> groupModels = get_models_group(group);
        models.insert(models.end(), groupModels.begin(), groupModels.end());
    }
    return models;
}

std::vector<TRANSFORM> get_group_transforms(GROUP g) {
    std::vector<TRANSFORM> transforms;
    for (const auto &transform : g.transforms)
        transforms.push_back(transform);
    return transforms;
}

std::vector<GROUP> get_group_children(GROUP g) {
    return g.children;
}

std::vector<GROUP> get_groups(WORLD w) {
    return w.groups;
}

std::vector<POINT> get_transform_points(TRANSFORM t) {
    return t.points;
}

POINT get_y_aux(TRANSFORM t){
    return t.y_aux;
}

void set_y_aux (TRANSFORM t, POINT y){
    set_X(t.y_aux, get_X(y));
    set_Y(t.y_aux, get_Y(y));
    set_Z(t.y_aux, get_Z(y));
}