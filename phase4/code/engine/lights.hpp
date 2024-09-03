// lights.hpp
#define GL_SILENCE_DEPRECATION
#ifndef LIGHTS_H
#define LIGHTS_H

enum LIGHT_TYPE {
    L_POINT,
    L_DIRECTIONAL,
    L_SPOTLIGHT
};

typedef struct light {
    LIGHT_TYPE type;
    union {
        struct {
            float posX;
            float posY;
            float posZ;
        } l_point;
        struct {
            float dirX;
            float dirY;
            float dirZ;
        } l_directional;
        struct {
            float posX;
            float posY;
            float posZ;
            float dirX;
            float dirY;
            float dirZ;
            float cutoff;
        } l_spotlight;  
    };
} LIGHT;

LIGHT init_light();

LIGHT new_light(LIGHT_TYPE type, float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float cutoff);

float get_light_posX( const LIGHT& light);
float get_light_posY( const LIGHT& light);
float get_light_posZ( const LIGHT& light);

float get_light_dirX( const LIGHT& light);
float get_light_dirY( const LIGHT& light);
float get_light_dirZ( const LIGHT& light);

float get_light_spot_posX( const LIGHT& light);
float get_light_spot_posY( const LIGHT& light);
float get_light_spot_posZ( const LIGHT& light);
float get_light_spot_dirX( const LIGHT& light);
float get_light_spot_dirY( const LIGHT& light);
float get_light_spot_dirZ( const LIGHT& light);
float get_light_spot_cutoff( const LIGHT& light);
void set_spotlight_z (LIGHT& light, float z);



#endif // LIGHTS_H