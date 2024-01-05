#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "estruturas.hpp"
#include "ListaRetas.hpp"
#include "ListaPontos.hpp"
#include "ListaPoligonos.hpp"


void drawButton(Button *button) {
    (button->ativo || button->pressed) == 1 ? glColor3d(button->rgb[0]-0.3,button->rgb[1]-0.3,button->rgb[2]-0.3) : glColor3dv(button->rgb);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(button->x, button->y, button->x + button->width, button->y + button->height);

    (button->ativo || button->pressed) == 1 ? glColor3d(0.7, 0.7, 0.7) : glColor3d(1, 1, 1);
    drawText(button->label);

}


EstadoExecucao *criar_execucao(EstadoExecucao *novo_estado){
    static int id = 0;

    //EstadoExecucao *novo_estado = (EstadoExecucao *) malloc(sizeof(EstadoExecucao));
    novo_estado->currentPage = homePage;
    novo_estado->pontos_criados = criarListaPontos();
    novo_estado->retas_criadas = criarListaRetas();
    novo_estado->poligonos_criados = criarListaPoligonos();
    novo_estado->lastButtonPressed = NULL;
    novo_estado->lastPointSelected = NULL;
    novo_estado->lastPolygonSelected = NULL;
    novo_estado->lastLineSelected = NULL;
    novo_estado->id = id;
    id++;

    return novo_estado;
}

void showPage(ProgramPage *pagina){

    if (pagina->showPageFunction != NULL){
        pagina->showPageFunction();
    }
    for (int c = 0; c < pagina->qtd_buttons; c++){
        drawButton(&pagina->pageButtons[c]);
    }
    for (int c = 0; c < pagina->qtd_texts; c++){
        drawText(pagina->pageTexts[c]);
    }

}

int criar_ponto(EstadoExecucao *estado_atual, double coord_x, double coord_y, double rgb_vector[3]){
    static int id = 0;
    Ponto novo_ponto;

    novo_ponto.id = id;
    novo_ponto.selected = 0;
    associar_ponto(&novo_ponto, coord_x, coord_y, rgb_vector);
    if (estado_atual != NULL){
        if(ListaPontosInserirFim(estado_atual->pontos_criados, novo_ponto))
            estado_atual->qtd_pontos++;
        else{
            printf("Erro ao criar ponto.\n");
            return 1;
        }
    }

    id++;
    return 0;
}

int associar_ponto(Ponto *destino, double x, double y, double rgb[3]){

    destino->x = x;
    destino->y = y;
    destino->rgb_color[0] = rgb[0];
    destino->rgb_color[1] = rgb[1];
    destino->rgb_color[2] = rgb[2];

    return 0;
}

int criar_pagina(ProgramPage *destino, int pageID, void (*pageFunction)(void)){
    destino->qtd_buttons = 0;
    destino->qtd_texts = 0;
    destino->pageID = pageID;
    destino->showPageFunction = pageFunction;
    return 0;
}

int criar_text(Text *destino, char *msg, void *font, double xStart, double yStart){
    strcpy(destino->texto, msg);
    destino->xStart = xStart;
    destino->yStart = yStart;
    destino->font = font;

    return 0;
}

int add_text(ProgramPage *pg, char *msg, void *font, double xStart, double yStart){
    Text *destino = &pg->pageTexts[pg->qtd_texts];
    strcpy(destino->texto, msg);
    destino->xStart = xStart;
    destino->yStart = yStart;
    destino->font = font;

    pg->qtd_texts++;
    return 0;
}

void drawText(Text texto){
    glRasterPos2f(texto.xStart, texto.yStart);
    for (int c = 0; c < (int) strlen(texto.texto); c++) {
        glutBitmapCharacter(texto.font, texto.texto[c]);
    }
}

int criar_reta(EstadoExecucao *estado_atual, Ponto ponto1, Ponto ponto2, double rgb_vector[3]){
    static int id = 0;
    Reta nova_reta;
    nova_reta.ponto1 = ponto1;
    nova_reta.ponto2 = ponto2;
    nova_reta.id = id;
    nova_reta.rgb_color[0] = rgb_vector[0];
    nova_reta.rgb_color[1] = rgb_vector[1];
    nova_reta.rgb_color[2] = rgb_vector[2];
    nova_reta.selected = 0;
    if (ListaRetasInserirFim(estado_atual->retas_criadas, nova_reta)){
        estado_atual->qtd_retas++;
    }
    else{
        printf("Erro ao criar reta.\n");
        return 1;
    }

    id++;
    return 0;
}

int criar_poligono(EstadoExecucao *estado_atual, ListaPontos *pontos, int qtd_pon_pol, double rgb_vector[3]){
    static int id = 0;

    Poligono novo_poligono;

    novo_poligono.id = id;
    novo_poligono.pontos = pontos;
    novo_poligono.qtd_pontos = qtd_pon_pol;
    novo_poligono.rgb_color[0] = rgb_vector[0];
    novo_poligono.rgb_color[1] = rgb_vector[1];
    novo_poligono.rgb_color[2] = rgb_vector[2];
    novo_poligono.selected = 0;
    if (ListaPoligonosInserirFim(estado_atual->poligonos_criados, novo_poligono)){
        estado_atual->qtd_poligonos++;
    }
    else{
        printf("Erro ao criar poligono.\n");
        return 1;
    }

    id++;
    return 0;
}

void criarBotao(ProgramPage *pg, int id, int x, int y, double width, double height, char *label, double rgb[3]){

    double paddingX = 20, paddingY = 10;
    Button *novo_botao = &pg->pageButtons[pg->qtd_buttons];
    double labelWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (unsigned char*) label), labelHeight = 12;

    novo_botao->id = id;
    novo_botao->x = x;
    novo_botao->y = y;
    novo_botao->width = (labelWidth + paddingX) > width ? labelWidth+paddingX : width;
    novo_botao->height = (labelHeight+paddingY) > height? labelHeight+paddingY : height;

    criar_text(&novo_botao->label, label, GLUT_BITMAP_HELVETICA_12, novo_botao->x + ((novo_botao->width - labelWidth) / 2), novo_botao->y + ((novo_botao->height - labelHeight) / 2));
    printf("%d %f\n w = %f lw = %f\n", novo_botao->x, novo_botao->width, novo_botao->label.xStart, labelWidth);
    for (int i = 0; i < 3; i++){
        novo_botao->rgb[i] = rgb[i];
    }

    novo_botao->ativo = 0;
    novo_botao->pressed = 0;
    pg->qtd_buttons++;
}

int isPointInsideButton(int x, int y, Button button) {
    if (button.height == 0){
        return 0;
    }

    if ((x >= button.x && x <= button.x + button.width) && (y >= button.y && y <= button.y + button.height)){
        return 1;
    }
    return 0;
}
