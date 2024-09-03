#include "color.hpp"

COLOR new_color(COLOR_TYPE type, float r, float g, float b, float value) {
    COLOR c;
    c.type = type;
    switch (type) {
        case C_DIFFUSE:
            c.diffuse.r = r;
            c.diffuse.g = g;
            c.diffuse.b = b;
            break;
        case C_AMBIENT:
            c.ambient.r = r;
            c.ambient.g = g;
            c.ambient.b = b;
            break;
        case C_SPECULAR:
            c.specular.r = r;
            c.specular.g = g;
            c.specular.b = b;
            break;
        case C_EMISSIVE:
            c.emissive.r = r;
            c.emissive.g = g;
            c.emissive.b = b;
            break;
        case C_SHININESS:
            c.shininess.value = value;
            break;
    }
    return c;
}

