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
    m.texture_file = "";
    m.colors = std::vector<COLOR>();
    return m;
}

GROUP create_new_group() {
    Group g;
    g.models = std::vector<Model>();
    g.transforms = std::vector<Transform>();
    g.children = std::vector<Group>();
    return g;
}

WORLD create_world() {
    WORLD w;
    w.windowWidth = 0;
    w.windowHeight = 0;
    w.camera = create_new_camera();
    w.groups = std::vector<GROUP>();
    w.lights = std::vector<LIGHT>();
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
            } else {
                std::cerr << "Error: Model element is missing 'file' attribute." << std::endl;
                continue; // Skip this model if 'file' attribute is missing
            }

            // Parse texture
            TiXmlElement* textureElement = modelElement->FirstChildElement("texture");
            if (textureElement) {
                const char* textureFileAttribute = textureElement->Attribute("file");
                if (textureFileAttribute) {
                    model.texture_file = textureFileAttribute;
                } else {
                    std::cerr << "Error: Texture element is missing 'file' attribute." << std::endl;
                }
            }

            // Parse colors
            for (TiXmlElement* colorElement = modelElement->FirstChildElement("color"); colorElement; colorElement = colorElement->NextSiblingElement("color"))
                parse_color_element(colorElement, model);

            group.models.push_back(model);
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

void parse_diffuse_color(TiXmlElement* colorElement, COLOR& color) {
    color.type = C_DIFFUSE;
    colorElement->QueryFloatAttribute("R", &color.diffuse.r);
    colorElement->QueryFloatAttribute("G", &color.diffuse.g);
    colorElement->QueryFloatAttribute("B", &color.diffuse.b);
}

void parse_ambient_color(TiXmlElement* colorElement, COLOR& color) {
    color.type = C_AMBIENT;
    colorElement->QueryFloatAttribute("R", &color.ambient.r);
    colorElement->QueryFloatAttribute("G", &color.ambient.g);
    colorElement->QueryFloatAttribute("B", &color.ambient.b);
}

void parse_specular_color(TiXmlElement* colorElement, COLOR& color) {
    color.type = C_SPECULAR;
    colorElement->QueryFloatAttribute("R", &color.specular.r);
    colorElement->QueryFloatAttribute("G", &color.specular.g);
    colorElement->QueryFloatAttribute("B", &color.specular.b);
}

void parse_emissive_color(TiXmlElement* colorElement, COLOR& color) {
    color.type = C_EMISSIVE;
    colorElement->QueryFloatAttribute("R", &color.emissive.r);
    colorElement->QueryFloatAttribute("G", &color.emissive.g);
    colorElement->QueryFloatAttribute("B", &color.emissive.b);
}

void parse_shininess_color(TiXmlElement* colorElement, COLOR& color) {
    color.type = C_SHININESS;
    colorElement->QueryFloatAttribute("value", &color.shininess.value);
}

void parse_color_element(TiXmlElement* colorElement, Model& model) {
    COLOR color;

    for (TiXmlElement* childElement = colorElement->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement()) {
        const char* colorType = childElement->Value();
        if (strcmp(colorType, "diffuse") == 0) {
            parse_diffuse_color(childElement, color);
        } else if (strcmp(colorType, "ambient") == 0) {
            parse_ambient_color(childElement, color);
        } else if (strcmp(colorType, "specular") == 0) {
            parse_specular_color(childElement, color);
        } else if (strcmp(colorType, "emissive") == 0) {
            parse_emissive_color(childElement, color);
        } else if (strcmp(colorType, "shininess") == 0) {
            parse_shininess_color(childElement, color);
        } else {
            std::cerr << "Error: Unknown color type '" << colorType << "'." << std::endl;
        }
        model.colors.push_back(color);
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
    world.lights.clear();
    // Parse lights
    for (TiXmlElement* lightsElement = worldElement->FirstChildElement("lights"); lightsElement; lightsElement = lightsElement->NextSiblingElement("lights")) {
        for (TiXmlElement* lightElement = lightsElement->FirstChildElement("light"); lightElement; lightElement = lightElement->NextSiblingElement("light")) {
            LIGHT light = init_light();
            const char* type = lightElement->Attribute("type");
                if (strcmp(type, "point") == 0) {
                    light.type = L_POINT;
                    lightElement->QueryFloatAttribute("posx", &light.l_point.posX);
                    lightElement->QueryFloatAttribute("posy", &light.l_point.posY);
                    lightElement->QueryFloatAttribute("posz", &light.l_point.posZ);
                } else if (strcmp (type, "directional") == 0) {
                    light.type = L_DIRECTIONAL;
                    lightElement->QueryFloatAttribute("dirx", &light.l_directional.dirX);
                    lightElement->QueryFloatAttribute("diry", &light.l_directional.dirY);
                    lightElement->QueryFloatAttribute("dirz", &light.l_directional.dirZ);
                } else if (strcmp (type, "spot") == 0) {
                    light.type = L_SPOTLIGHT;
                    lightElement->QueryFloatAttribute("posx", &light.l_spotlight.posX);
                    lightElement->QueryFloatAttribute("posy", &light.l_spotlight.posY);
                    lightElement->QueryFloatAttribute("posz", &light.l_spotlight.posZ);
                    lightElement->QueryFloatAttribute("dirz", &light.l_spotlight.dirX);
                    lightElement->QueryFloatAttribute("diry", &light.l_spotlight.dirY);
                    lightElement->QueryFloatAttribute("dirz", &light.l_spotlight.dirZ);
                    lightElement->QueryFloatAttribute("cutoff", &light.l_spotlight.cutoff);
                } else {
                    // Handle unknown light type
                    std::cerr << "Unknown light type: " << type << std::endl;
                }
                //printf ("Position: %f %f %f\n", light.l_point.posX, light.l_point.posY, light.l_point.posZ);
                //printf ("Direction: %f %f %f\n", light.l_directional.dirX, light.l_directional.dirY, light.l_directional.dirZ);
                //printf ("Spotlight: %f %f %f %f %f %f %f\n", light.l_spotlight.posX, light.l_spotlight.posY, light.l_spotlight.posZ, light.l_spotlight.dirX, light.l_spotlight.dirY, light.l_spotlight.dirZ, light.l_spotlight.cutoff);
                world.lights.push_back(light);
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
    for (Group &group : w.groups) {
        for (Model &model : group.models) {
            model.file = "";
            model.texture_file = "";
            model.colors.clear();
        }
        group.models.clear(); // Limpar os modelos do grupo
        group.transforms.clear(); // Limpar as transformações do grupo
        group.children.clear(); // Limpar os filhos do grupo
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

std::vector<LIGHT> get_lights(WORLD w) {
    return w.lights;
}

std::vector<COLOR> get_colors (MODEL m){
    return m.colors;
}

std::vector<string> get_textures(WORLD w) {
    std::vector<string> textures;
    for (const auto &group : w.groups) {
        for (const auto &model : group.models) {
            if (!model.texture_file.empty()) {
                textures.push_back(model.texture_file);
            }
        }
    }
    return textures;
}