#include "lights.hpp"
LIGHT init_light() {
    LIGHT l;
    l.l_point.posX = 0.0f;
    l.l_point.posY = 0.0f;
    l.l_point.posZ = 0.0f;
    l.l_directional.dirX = 0.0f;
    l.l_directional.dirY = 0.0f;
    l.l_directional.dirZ = 0.0f;
    l.l_spotlight.posX = 0.0f;
    l.l_spotlight.posY = 0.0f;
    l.l_spotlight.posZ = 0.0f;
    l.l_spotlight.dirX = 0.0f;
    l.l_spotlight.dirY = 0.0f;
    l.l_spotlight.dirZ = 0.0f;
    l.l_spotlight.cutoff = 0.0f;
    return l;
}

LIGHT new_light(LIGHT_TYPE type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float cutoff) {
    LIGHT l;
    l.type = type;
    switch (type) {
        case L_POINT:
            l.l_point.posX = posX;
            l.l_point.posY = posY;
            l.l_point.posZ = posZ;
            break;
        case L_DIRECTIONAL:
            l.l_directional.dirX = dirX;
            l.l_directional.dirY = dirY;
            l.l_directional.dirZ = dirZ;
            break;
        case L_SPOTLIGHT:
            l.l_spotlight.posX = posX;
            l.l_spotlight.posY = posY;
            l.l_spotlight.posZ = posZ;
            l.l_spotlight.dirX = dirX;
            l.l_spotlight.dirY = dirY;
            l.l_spotlight.dirZ = dirZ;
            l.l_spotlight.cutoff = cutoff;
            break;
    }
    return l;
}

// Get the type of the light
LIGHT_TYPE get_light_type(const LIGHT& light) {
    return light.type;
}

// Getters for point light
float get_light_posX(const LIGHT& light) {
    return light.l_point.posX;
}

float get_light_posY(const LIGHT& light) {
    return light.l_point.posY;
}

float get_light_posZ(const LIGHT& light) {
    return light.l_point.posZ;
}

// Getters for directional light
float get_light_dirX(const LIGHT& light) {
    return light.l_directional.dirX;
}

float get_light_dirY(const LIGHT& light) {
    return light.l_directional.dirY;
}

float get_light_dirZ(const LIGHT& light) {
    return light.l_directional.dirZ;
}

// Getters for spotlight
float get_light_spot_posX(const LIGHT& light) {
    return light.l_spotlight.posX;
}

float get_light_spot_posY(const LIGHT& light) {
    return light.l_spotlight.posY;
}

float get_light_spot_posZ(const LIGHT& light) {
    return light.l_spotlight.posZ;
}

float get_light_spot_dirX(const LIGHT& light) {
    return light.l_spotlight.dirX;
}

float get_light_spot_dirY(const LIGHT& light) {
    return light.l_spotlight.dirY;
}

float get_light_spot_dirZ(const LIGHT& light) {
    return light.l_spotlight.dirZ;
}

float get_light_spot_cutoff(const LIGHT& light) {
    return light.l_spotlight.cutoff;
}

void set_spotlight_z (LIGHT& light, float z) {
    light.l_spotlight.posZ = z;
}

