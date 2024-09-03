#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <list>
#include <cmath>
#include "xml_parser.hpp"
#include "../utils/figure.hpp"
#include "../utils/triangle.hpp"

#define RED 1.0f, 0.0f, 0.0f
#define GREEN 0.0f, 1.0f, 0.0f
#define BLUE 0.0f, 0.0f, 1.0f
#define WHITE 1.0f, 1.0f, 1.0f
#define PINK 1.0f, 0.75f, 0.8f
#define PURPLE 0.5f, 0.0f, 0.5f

WORLD world = create_world();
std::list<FIGURE> figures_list;

float camX, camY, camZ; //camera
float LAX, LAY, LAZ; //look at
float upX, upY, upZ; //up
float fov, near, far;
float alpha = M_PI / 4, beta = M_PI / 4;
float radius;

void changeSize(int w, int h) {
    if (h == 0)
        h = 1;

    float ratio = w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
}

void draw_axis() {
    glBegin(GL_LINES);
    // X Axis
    glColor3f(PINK);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    // Y Axis
    glColor3f(PURPLE);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    // Z Axis
    glColor3f(BLUE);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glEnd();
}

void draw_figures(std::list<FIGURE> figs_list) {
    glBegin(GL_TRIANGLES);
    for (std::list<FIGURE>::iterator it = figs_list.begin(); it != figs_list.end(); ++it) {
        // Check if figure is not NULL
        if (*it != nullptr) {
            std::vector<TRIANGLE>* triangles = get_triangles(*it);
            for (TRIANGLE t : *triangles) {
                for (POINT p : *get_points(t)) {
                    glVertex3f(get_X(p), get_Y(p), get_Z(p));
                }
            }
        }
    }
    glEnd();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //gluLookAt(radius*cos(beta)*sin(alpha), radius*sin(beta), radius*cos(beta)*cos(alpha), LAX,LAY,LAZ,upX,upY,upZ);
    gluLookAt(radius*camX, radius*camY, radius*camZ, LAX, LAY, LAZ, upX, upY, upZ);

    draw_axis();
    glColor3f(WHITE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw_figures(figures_list);
    glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        camX -= 0.5;
        break;
    case 'd':
        camX += 0.5;
        break;
    case 's':
        camZ += 0.5;
        break;
    case 'w':
        camZ -= 0.5;
        break;
    /*case 'q':
        alpha -= 0.1;
        break;
    case 'e':
        alpha += 0.1;
        break;*/
    case 'i':
        camY += 0.5;
        break;
    case 'k':
        camY -= 0.5;
        break;
    /*case 'j':
        LAX -= 0.5;
        break;
    case 'l':
        LAX += 0.5;
        break;*/
    case 'u':
        LAY -= 0.5;
        break;
    case 'o':
        LAY += 0.5;
        break;
    case '+':
        radius += 0.1;
        break;
    case '-':
        radius -= 0.1;
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Phase1");

    parse_config_file(argv[1], world);
    camX = get_position_camX(world);
    camY = get_position_camY(world);
    camZ = get_position_camZ(world);
    LAX = get_lookAt_camX(world);
    LAY = get_lookAt_camY(world);
    LAZ = get_lookAt_camZ(world);
    upX = get_up_camX(world);
    upY = get_up_camY(world);
    upZ = get_up_camZ(world);
    fov = get_fov(world);
    near = get_near(world);
    far = get_far(world);
	radius = sqrt(camX * camX + camY * camY + camZ * camZ);
    alpha = acos(camZ / radius);
    beta = asin(camY / radius);

    std::vector<Model> models = get_models(world);
	int num_models = models.size();

	for (int i = 0; i < num_models; i++) 
    	figures_list.push_back(fileToFigure(models[i].file));


    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboardFunc);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}
