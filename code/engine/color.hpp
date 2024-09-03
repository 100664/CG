// color.hpp
#define GL_SILENCE_DEPRECATION
#ifndef COLOR_H
#define COLOR_H

enum COLOR_TYPE {
    C_DIFFUSE,
    C_AMBIENT,
    C_SPECULAR,
    C_EMISSIVE,
    C_SHININESS
};

typedef struct color {
    COLOR_TYPE type;
    union {
        struct {
            float r;
            float g;
            float b;
        } diffuse;
        struct {
            float r;
            float g;
            float b;
        } ambient; 
        struct {
            float r;
            float g;
            float b;
        } specular; 
        struct {
            float r;
            float g;
            float b;
        } emissive; 
        struct {
            float value;
        } shininess; 
    };
} COLOR;

#endif //   COLOR_H