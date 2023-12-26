#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.hpp"
#include "ListaPontos.hpp"
#include "ListaRetas.hpp"
#include "ListaPoligonos.hpp"

EstadoExecucao *execucao = criar_execucao();
int desenhaPontos(ListaPontos *ldse);

void display(){

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    desenhaPontos(execucao->pontos_criados);
    desenhaRetas(execucao->retas_criadas);
    desenhaPoligonos(execucao->poligonos_criados);

    glFlush(); //desenha os comandos não executados
}

void init(){

    glClearColor(1.0, 1.0, 1.0, 1.0); //define a cor de fundo

    glMatrixMode(GL_PROJECTION); //carrega a matriz de projeção
    glOrtho(0, 100, 0, 100, -1.0, 1.0); //define a projeção ortogonal 2D

    double rgb[] = {0.0, 0.0, 1.0};
    Ponto *novo_ponto = criar_ponto(execucao, 15.0, 5, rgb);
    Ponto *ponto2 = criar_ponto(execucao, 1.0, 10.0, rgb);
    Ponto *ponto3 = criar_ponto(execucao, 10.0, 30.0, rgb);
    Ponto *ponto4 = criar_ponto(execucao, 60.0, 40.0, rgb);

    rgb[2] = 0;
    rgb[0] = 1;
    Reta *reta1 = criar_reta(execucao, *ponto2, *novo_ponto, rgb);

    rgb[0] = 0;
    rgb[1] = 1;
    Poligono *poligono1 = criar_poligono(execucao, execucao->pontos_criados, execucao->qtd_pontos, rgb);

    //imprimeListaPontos(execucao->pontos_criados);

    //imprimeListaRetas(execucao->retas_criadas);

    //imprimeListaPoligonos(execucao->poligonos_criados);
}


int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Paint");

    init();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
