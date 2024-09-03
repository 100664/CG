#include "box.hpp"

FIGURE generate_box(float length, int divisions) {
    FIGURE box = create_figure_plane_box(BOX, length, divisions);
    if (box){
    float dimension2 = (float)length / 2; 
    FIGURE faceCima = generate_plane_XZ(length, divisions, dimension2, 0);
    FIGURE faceBaixo = generate_plane_XZ(length, divisions, -dimension2, 1);
    FIGURE faceLateral1 = generate_plane_XY(length, divisions, -dimension2, 0);
    FIGURE faceLateral2 = generate_plane_XY(length, divisions, dimension2, 1);
    FIGURE faceLateral3 = generate_plane_YZ(length, divisions, -dimension2, 0);
    FIGURE faceLateral4 = generate_plane_YZ(length, divisions, dimension2, 1);

    concat_FIGURES(box, faceCima);
    concat_FIGURES(box, faceBaixo);
    concat_FIGURES(box, faceLateral1);
    concat_FIGURES(box, faceLateral2);
    concat_FIGURES(box, faceLateral3);
    concat_FIGURES(box, faceLateral4);

    free_figure(faceCima);
    free_figure(faceBaixo);
    free_figure(faceLateral1);
    free_figure(faceLateral2);
    free_figure(faceLateral3);
    free_figure(faceLateral4);
    }
    else{
        printf("Error: Box not created\n");
    }
    return box;
}