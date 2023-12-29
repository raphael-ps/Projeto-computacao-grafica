#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "myUtils.hpp"
#include "estruturas.hpp"
#include "ListaPontos.hpp"
#include "ListaRetas.hpp"
#include "ListaPoligonos.hpp"

#define MAX_BUTTONS 10

EstadoExecucao estado_atual;

const int windowSizeX = 800, windowSizeY = 600;
const int ortoSizeX = windowSizeX, ortoSizeY = windowSizeY;
double rgb[3] = {0, 0, 1};


int criarNovoArquivo(EstadoExecucao *nova_execucao){
    criar_execucao(nova_execucao);
    nova_execucao->modo_execucao = 1;
    return 0;
}

int carregarArquivo(EstadoExecucao *execucao){
    FILE *fp = fopen("C:\\Users\\Softex\\Documents\\GitHub\\Projeto-computacao-grafica\\estado_execucao", "rb");

    if (fp == NULL) {
        printf("O arquivo não existe, um novo será criado\n");
        execucao = criar_execucao(execucao);
    }
    else{
        int tamanho = sizeof(EstadoExecucao);
        fread(execucao, tamanho, 1, fp);

        carregarListaPontos(fp, execucao);
        carregarListaRetas(fp, execucao);
        carregarListaPoligonos(fp, execucao);

        printf("Arquivo Carregado!\n");
        fclose(fp);
    }

    execucao->modo_execucao = 1;
    return 0;
}

int salvarArquivo(EstadoExecucao *execucao){
    printf("qtd pontos salvar = %d\n", execucao->qtd_pontos);
    FILE *fp = fopen("C:\\Users\\Softex\\Documents\\GitHub\\Projeto-computacao-grafica\\estado_execucao", "w+b");
    if (fp == NULL){
        printf("Erro ao tentar salvar arquivo!\n");
        return 1;
    }

    int tamanho = sizeof(EstadoExecucao);
    fwrite(execucao, tamanho, 1, fp);

    salvarListaPontos(fp, execucao->pontos_criados);
    salvarListaRetas(fp, execucao->retas_criadas);
    salvarListaPoligonos(fp, execucao->poligonos_criados);

    printf("Arquivo Salvo!\n");
    fclose(fp);
    printf("Modo Exec = %d\n", estado_atual.modo_execucao);
    return 0;
}

Button botoes[MAX_BUTTONS];

void init_vetor_botoes(){
    for (int c = 0; c < MAX_BUTTONS; c++){
        botoes[c].height = 0;
    }
}


void mouseClickHandler(int button, int state, int x, int y) {

    y = (ortoSizeY - y);

    // Check if the left mouse button was clicked and released
    if (estado_atual.modo_execucao == 0 && button == GLUT_LEFT_BUTTON && state == 1) {
        Button botao_clicado;

        for (int c = 0; c < MAX_BUTTONS; c++){
            if (isPointInsideButton(x, y, botoes[c])){
                botao_clicado = botoes[c];
                printf("Botao -%s- clicado\n", botoes[c].label);
                break;
            }
        }

        switch(botao_clicado.id){

        case 1:
            criarNovoArquivo(&estado_atual);
            break;
        case 2:
            carregarArquivo(&estado_atual);
            break;
        default:
            break;
        }

    }
    else if (estado_atual.modo_execucao == 1 && button == GLUT_LEFT_BUTTON && state == 1){
        double prgb[] = {1, 0, 0};
        criar_ponto(&estado_atual, x, y, prgb);
    }
}

void drawButton(Button *button, void *font) {
    button->ativo == 0 ? glColor3dv(button->rgb) : glColor3d(button->rgb[0]-0.3,button->rgb[1]-0.3,button->rgb[2]-0.3);

    double paddingX = 20, paddingY = 10;
    double labelWidth = glutBitmapLength(font, (const unsigned char*) button->label);
    double labelHeight = 14;

    if ((labelWidth + paddingX) > button->width){
        button->width = labelWidth+paddingX;
    }

    if ((labelHeight+paddingY) > button->height){
        button->height = labelHeight+paddingY;
    }

    glRectf(button->x, button->y, button->x + button->width, button->y + button->height);

    button->ativo == 1 ? glColor3d(0.7, 0.7, 0.7) : glColor3d(1, 1, 1);
    glRasterPos2f(button->x + ((button->width - labelWidth) / 2), button->y + ((button->height - labelHeight) / 2));
    for (int c = 0; c < (int) strlen(button->label); c++) {
        glutBitmapCharacter(font, button->label[c]);
    }
}


void drawMenu() {
    glutMouseFunc(mouseClickHandler);

  drawButton(&botoes[0], GLUT_BITMAP_HELVETICA_18);
  drawButton(&botoes[1], GLUT_BITMAP_HELVETICA_18);

}

void display(){

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

    //printf("Modo Exec = %d\n", estado_atual.modo_execucao);

    if ( estado_atual.modo_execucao == 1 ){
        desenhaPontos(estado_atual.pontos_criados);
    } else {
        drawMenu();
    }

  glFlush(); //desenha os comandos não executados
}

void init(){

    glClearColor(0, 0, 0, 1.0); //define a cor de fundo

    glMatrixMode(GL_PROJECTION); //carrega a matriz de projeção
    glOrtho(0, ortoSizeX, 0, ortoSizeY, -1.0, 1.0); //define a projeção ortogonal 2D

    glutMouseFunc(mouseClickHandler);
    criarBotao(&botoes[0], 1, 50, ortoSizeY-50, 100, 30, "Novo Documento", rgb);
    rgb[0] = 1;
    criarBotao(&botoes[1], 2, 50, ortoSizeY-90, 100, 30, "Abrir Documento", rgb);
}

void mouseEnterButton(int x , int y){
    y = (ortoSizeY - y);


    for (int c = 0; c < MAX_BUTTONS; c++){
        if (isPointInsideButton(x, y, botoes[c]) && botoes[c].ativo == 0){
            botoes[c].ativo = 1;
        }
        if (!isPointInsideButton(x, y, botoes[c]) && botoes[c].ativo == 1){
            botoes[c].ativo = 0;
        }
    }
    glutPostRedisplay();
}

void exitCallback(){
     salvarArquivo(&estado_atual);
}

int main(int argc, char** argv){
    init_vetor_botoes();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowSizeX, windowSizeY);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Paint");
    glutPassiveMotionFunc(mouseEnterButton);
    atexit(exitCallback);

    init();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
