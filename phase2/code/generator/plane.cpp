#include "plane.hpp"
                                                                          //o length é o tamanho do lado do plano
                                                                          //o divisions é o número de divisões do plano
FIGURE generate_plane_XZ(int length, int divisions, float h, int baixo) { 
    //o h é a altura do plano em relação ao eixo y
                                                                          //o baixo é para inverter o plano
    FIGURE f = create_figure_plane_box(PLANE, length, divisions); 

    float dimension = (float)length / 2; //isto serve para centrar o plano no centro do eixo (0,0,0) 
    float division = (float)length / divisions; //isto serve para dividir o plano nas várias partes que queremos gerar

    //agr vamos criar os pontos do plano, depois de sabermos quantas divisões queremos e termos a referença do centro do plano

    float x1 = -dimension, z1 = -dimension;                         //isto é o ponto inicial do plano, ou seja, está no canto inferior esquerdo
    float x2 = -dimension, z2 = -dimension + division;              //isto é o ponto que está à direita do ponto inicial, ou seja, está no canto superior esquerdo
    float x3 = -dimension + division, z3 = -dimension;              //isto é o ponto que está em baixo do ponto inicial, ou seja, está no canto inferior direito
    float x4 = -dimension + division, z4 = -dimension + division;   //isto é o ponto que está à direita do ponto inicial, ou seja, está no canto superior direito

    float Xs[4] = {x1, x2, x3, x4}; //array dos X's
    float Zs[4] = {z1, z2, z3, z4}; //arrays dos Z's

    //isto serve para inverter, pois vamos usar isto para criar a box (verificar mesmo se queremos usar isto, caso não seja necessário, apagar)
    if (baixo == 1){
        Xs[1] = x3;
        Xs[2] = x2;
        Zs[1] = z3;
        Zs[2] = z2;
    } 

    //ao usarmos estes dois ciclos for, vamos criar os vários pontos do plano de maneira a que fiquem divididos nas várias partes que queremos
   // printf("XZ\n");
    for (int linha = 0; linha < divisions; linha++){ //itera sobre o eixo do Z (neste caso)
        for (int coluna = 0; coluna < divisions; coluna++){ //itera sobre o eixo do X
            // Primeiro triângulo do quadrado

            //criar o ponto 1, como é que isto funciona? (Xs[0] + coluna * division) é o que? e (h, Zs[0])?
            // o coluna * division é para nos dar a partição à qual fará parte o ponto, como se fosse um ponto de referência
            // o h é a altura do plano em relação ao eixo y
            // o Zs[0] é o ponto inicial do plano, ou seja, está no canto inferior esquerdo

            //isto vai ser replicado para os outros pontos
            POINT p1 = new_point(Xs[0] + coluna * division, h, Zs[0]); 
            POINT p2 = new_point(Xs[1] + coluna * division, h, Zs[1]);
            POINT p3 = new_point(Xs[2] + coluna * division, h, Zs[2]);
            // Segundo triângulo do quadrado
            POINT p4 = new_point(Xs[1] + coluna * division, h, Zs[1]);
            POINT p5 = new_point(Xs[3] + coluna * division, h, Zs[3]);
            POINT p6 = new_point(Xs[2] + coluna * division, h, Zs[2]);

            /*print_point(p1);
            print_point(p2);
            print_point(p3);

            print_point(p4);
            print_point(p5);
            print_point(p6);*/

            // Inicialize vetores vazios
            std::vector<POINT> vertices1;
            std::vector<POINT> vertices2; 

            // Adicione os elementos um a um
            vertices1.push_back(p1); //adiciona o ponto 1 à lista de vertices
            vertices1.push_back(p2); //...
            vertices1.push_back(p3); //...
            vertices2.push_back(p4); //...
            vertices2.push_back(p5); //...
            vertices2.push_back(p6); //...


            TRIANGLE t1 = create_triangle_with_vertices(vertices1);
            TRIANGLE t2 = create_triangle_with_vertices(vertices2);

            add_triangle(f, t1);
            add_triangle(f, t2);

        }
        Zs[0] += division; //aumentamos o Z do ponto inicial
        Zs[1] += division; //...
        Zs[2] += division; //...
        Zs[3] += division; //...

        //isto vai servir para criar a próxima "resma" de pontos
    }

    return f;
}

FIGURE generate_plane_XY(int length, int divisions, float h, int baixo) {
    FIGURE f = create_figure_plane_box(PLANE, length, divisions);

    float dimension = (float)length / 2;
    float division = (float)length / divisions;
    float x1 = -dimension, y1 = -dimension;
    float x2 = -dimension, y2 = -dimension + division;
    float x3 = -dimension + division, y3 = -dimension;
    float x4 = -dimension + division, y4 = -dimension + division;

    float Xs[4] = {x1, x2, x3, x4};
    float Ys[4] = {y1, y2, y3, y4};

    if (baixo == 1){
        Xs[1] = x3;
        Xs[2] = x2;
        Ys[1] = y3;
        Ys[2] = y2;
    }
    //printf("XY\n");
    for (int linha = 0; linha < divisions; linha++){
        for (int coluna = 0; coluna < divisions; coluna++){
            // Primeiro triângulo do quadrado
            POINT p1 = new_point(Xs[0] + coluna * division, Ys[0], h);
            POINT p2 = new_point(Xs[1] + coluna * division, Ys[1], h);
            POINT p3 = new_point(Xs[2] + coluna * division, Ys[2], h);
            // Segundo triângulo do quadrado
            POINT p4 = new_point(Xs[1] + coluna * division, Ys[1], h);
            POINT p5 = new_point(Xs[3] + coluna * division, Ys[3], h);
            POINT p6 = new_point(Xs[2] + coluna * division, Ys[2], h);



            // Inicialize vetores vazios
            std::vector<POINT> vertices1;
            std::vector<POINT> vertices2; 


            // Adicione os elementos um a um
            vertices1.push_back(p1); //adiciona o ponto 1 à lista de vertices
            vertices1.push_back(p2); //...
            vertices1.push_back(p3); //...
            vertices2.push_back(p4); //...
            vertices2.push_back(p5); //...
            vertices2.push_back(p6); //...


            TRIANGLE t1 = create_triangle_with_vertices(vertices1);
            TRIANGLE t2 = create_triangle_with_vertices(vertices2);

            add_triangle(f, t1);
            add_triangle(f, t2);

        }
        Ys[0] += division;
        Ys[1] += division;
        Ys[2] += division;
        Ys[3] += division;
    }

    return f;
}

FIGURE generate_plane_YZ(int length, int divisions, float h, int baixo) {
    FIGURE f = create_figure_plane_box(PLANE, length, divisions);

    float dimension = (float)length / 2;
    float division = (float)length / divisions;
    float y1 = -dimension, z1 = -dimension;
    float y2 = -dimension, z2 = -dimension + division;
    float y3 = -dimension + division, z3 = -dimension;
    float y4 = -dimension + division, z4 = -dimension + division;

    float Ys[4] = {y1, y2, y3, y4};
    float Zs[4] = {z1, z2, z3, z4};

    if (baixo == 1){
        Ys[1] = y3;
        Ys[2] = y2;
        Zs[1] = z3;
        Zs[2] = z2;
    }
    //printf ("YZ\n");
    for (int linha = 0; linha < divisions; linha++){
        for (int coluna = 0; coluna < divisions; coluna++){
            // Primeiro triângulo do quadrado
            POINT p1 = new_point(h, Ys[0] + coluna * division, Zs[0]);
            POINT p2 = new_point(h, Ys[1] + coluna * division, Zs[1]);
            POINT p3 = new_point(h, Ys[2] + coluna * division, Zs[2]);
            // Segundo triângulo do quadrado
            POINT p4 = new_point(h, Ys[1] + coluna * division, Zs[1]);
            POINT p5 = new_point(h, Ys[3] + coluna * division, Zs[3]);
            POINT p6 = new_point(h, Ys[2] + coluna * division, Zs[2]);
            
           

            // Inicialize vetores vazios
            std::vector<POINT> vertices1;
            std::vector<POINT> vertices2;

            vertices1.push_back(p1); //adiciona o ponto 1 à lista de vertices
            vertices1.push_back(p2); //...
            vertices1.push_back(p3); //...
            vertices2.push_back(p4); //...
            vertices2.push_back(p5); //...
            vertices2.push_back(p6); //...
            

            // Adicione os elementos um a um
            TRIANGLE t1 = create_triangle_with_vertices(vertices1);
            TRIANGLE t2 = create_triangle_with_vertices(vertices2);

            add_triangle(f, t1);
            add_triangle(f, t2);

        }
        Zs[0] += division;
        Zs[1] += division;
        Zs[2] += division;
        Zs[3] += division;
    }

    return f;
}

