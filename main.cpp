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

void drawMenu();
void drawErrorPage();

EstadoExecucao estado_atual;
ProgramPage paginaHome, errorPage, drawingPage;

const int windowSizeX = 800, windowSizeY = 600;
const int ortoSizeX = windowSizeX, ortoSizeY = windowSizeY;
double rgb[3] = {0, 0, 1};

int showPontosPoliProgress = 0, tempListQtdPontos = 0;
ListaPontos *tempListPontosPoli = NULL;

void translatePoint(Ponto *p, int direction){
    int step = 3;
    printf("%d key pressed\n", direction);

    switch (direction){
    case 119:
        p->y += step;
        printf("w %d\n", direction);
        break;
    case 97:
        p->x -= step;
        printf("a %d\n", direction);
        break;
    case 115:
        p->y -= step;
        printf("s %d\n", direction);
        break;
    case 100:
        p->x += step;
        printf("d %d\n", direction);
        break;
    }
}

void translatePolygon(Poligono *poly, int direction){
    ListaPontos *pontosPoli = poly->pontos;
    ListaPontos first = *pontosPoli;

    while (first != NULL){
        translatePoint(&first->ponto, direction);
        first = first->proximo;
    }
}

void translateLine(Reta *r, int direction){
    translatePoint(&r->ponto1, direction);
    translatePoint(&r->ponto2, direction);
}

void handleKeyboardDownIn(unsigned char key, int x, int y){
    if (estado_atual.lastPointSelected != NULL){
        translatePoint(estado_atual.lastPointSelected, key);
    }
    else if (estado_atual.lastLineSelected){
        translateLine(estado_atual.lastLineSelected, key);
    } else if (estado_atual.lastPolygonSelected != NULL){
        translatePolygon(estado_atual.lastPolygonSelected, key);
    }
    glutPostRedisplay();
}

void handleKeyboardUpIn(unsigned char key, int x, int y) {
    printf("%c %d ------0-0-\n", key, (int) key);

    if ((int) key == 127){

        if (estado_atual.lastButtonPressed != NULL &&
            estado_atual.lastButtonPressed->id == cliqueSelecionarobjeto && estado_atual.currentPage == drawPage){
            if(estado_atual.lastPointSelected != NULL){
                if (ListaPontosRemoverValor(estado_atual.pontos_criados, estado_atual.lastPointSelected->id) ){
                    estado_atual.lastPointSelected = NULL;
                    estado_atual.qtd_pontos--;
                };

            } else if (estado_atual.lastLineSelected != NULL){

                if (ListaRetasRemoverValor(estado_atual.retas_criadas, estado_atual.lastLineSelected->id) ){

                    estado_atual.lastLineSelected = NULL;
                    estado_atual.qtd_retas--;
                };
            } else if (estado_atual.lastPolygonSelected != NULL){
                if (ListaPoligonosRemoverValor(estado_atual.poligonos_criados, estado_atual.lastPolygonSelected->id) ){
                    estado_atual.lastPolygonSelected = NULL;
                    estado_atual.qtd_poligonos--;
                };
            }
            glutPostRedisplay();
        }
    }
}

void handleObjectsSelection(int mouseX, int mouseY){

    estado_atual.lastPointSelected = pickPontoIteration(estado_atual.pontos_criados, mouseX, mouseY);

    //if(estado_atual.lastPointSelected){
     //   lastPointSel = pointSelected;
    //}
    if (!estado_atual.lastPointSelected){

        estado_atual.lastLineSelected = pickLineIteration(estado_atual.retas_criadas, mouseX, mouseY);

        if (!estado_atual.lastLineSelected) {
            estado_atual.lastPolygonSelected = pickPolygonIteration(estado_atual.poligonos_criados, mouseX, mouseY);
        }
    }
}

void handleCliqueCriarPontos(int x, int y){
    double prgb[3] = {1, 0, 0};
    criar_ponto(&estado_atual, x, y, prgb);
}

void handleCliqueCriarRetas(int x, int y){
    double rrgb[3] = {0, 0.5, 0.5};
    static Ponto ponto1, ponto2;
    static int qtd = 0;

    if(qtd % 2 == 0){
        ponto1.id = 11;
        associar_ponto(&ponto1, x, y, rrgb);
        qtd++;
    } else {
        ponto2.id = 12;
        associar_ponto(&ponto2, x, y, rrgb);
        criar_reta(&estado_atual, ponto1, ponto2, rrgb);

        qtd = 0;
    }
}

void handleCliqueCriarPoligonos(int x, int y, int fimDaCaptura){
    double polirgb[3] = {0, 1, 0};
    showPontosPoliProgress = 1; //Global Var

    if (tempListPontosPoli == NULL){ //Global Var
        tempListPontosPoli = criarListaPontos();
    }

    if(!fimDaCaptura){
        Ponto pontoCapturado;
        pontoCapturado.id = 44;
        associar_ponto(&pontoCapturado, x, y, polirgb);
        tempListQtdPontos++; //Global Var
        ListaPontosInserirFim(tempListPontosPoli, pontoCapturado);
    } else {
        if (tempListQtdPontos >= 3){
            criar_poligono(&estado_atual, tempListPontosPoli, tempListQtdPontos, polirgb);
        }
        else{
            destruirListaPontos(tempListPontosPoli);
        }
        tempListPontosPoli = NULL;
        tempListQtdPontos = 0;
        showPontosPoliProgress = 0;
    }
}

int criarNovoArquivo(EstadoExecucao *nova_execucao){
    printf("Novo arquivo\n");
    criar_execucao(nova_execucao);
    nova_execucao->currentPage = drawPage;
    printf("current = %d\n", estado_atual.currentPage);
    return 0;
}

int carregarArquivo(EstadoExecucao *execucao){
    FILE *fp = fopen("estado_execucao", "rb");

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

    execucao->currentPage = drawPage;
    return 0;
}

int salvarArquivo(EstadoExecucao *execucao){
    printf("qtd pontos salvar = %d\n", execucao->qtd_pontos);
    printf("qtd retas salvar = %d\n", execucao->qtd_retas);
    printf("qtd poli salvar = %d\n", execucao->qtd_poligonos);

    FILE *fp = fopen("estado_execucao", "w+b");
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

    return 0;
}
/**
void init_vetor_botoes(){
    for (int c = 0; c < MAX_BUTTONS; c++){
        botoes[c].height = -1;
    }
}**/

ProgramPage *getCurrentPage(){
    ProgramPage *current;

    switch(estado_atual.currentPage){
        case homePage:
            current = &paginaHome;
            break;
        case drawPage:
            current = &drawingPage;
            break;
        default:
            current = &errorPage;
    }

    return current;

}

void mouseClickHandler(int button, int state, int x, int y) {
    printf("HERE\n");
    y = (ortoSizeY - y);
    ProgramPage *currentPage = getCurrentPage();
    int houve_botao_clicado = 0;

    // Check if the left mouse button was clicked and released
    if (estado_atual.lastLineSelected){
        estado_atual.lastLineSelected->selected = 0;
        estado_atual.lastLineSelected = NULL;
    }
    if (estado_atual.lastPointSelected){
        estado_atual.lastPointSelected->selected = 0;
        estado_atual.lastPointSelected = NULL;
    }
    if (estado_atual.lastPolygonSelected){
        estado_atual.lastPolygonSelected->selected = 0;
        estado_atual.lastPolygonSelected = NULL;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

        for (int c = 0; c < currentPage->qtd_buttons; c++){
            if (isPointInsideButton(x, y, currentPage->pageButtons[c])){
                if (estado_atual.lastButtonPressed != NULL){
                    estado_atual.lastButtonPressed->pressed = 0;
                }
                estado_atual.lastButtonPressed = &currentPage->pageButtons[c];
                estado_atual.lastButtonPressed->pressed = 1;
                printf("Botao -%s- clicado\n", currentPage->pageButtons[c].label.texto);
                houve_botao_clicado = 1;
                break;
            }
        }

        if (!houve_botao_clicado && estado_atual.currentPage == drawPage){
            switch(estado_atual.lastButtonPressed->id){
            case cliqueCriarPontos:
                handleCliqueCriarPontos(x, y);
                break;
            case cliqueCriarRetas:
                handleCliqueCriarRetas(x, y);
                break;
            case cliqueCriarPoligonos:
                handleCliqueCriarPoligonos(x, y, 0);
                break;
            case cliqueSelecionarobjeto:
                handleObjectsSelection(x, y);
                break;
            }
            return;
        }

        if (!houve_botao_clicado)
            return;

        switch(estado_atual.lastButtonPressed == NULL ? -1 : estado_atual.lastButtonPressed->id){

        case cliqueNovoArquivo:
            criarNovoArquivo(&estado_atual);
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            break;
        case cliqueCarregarArquivo:
            carregarArquivo(&estado_atual);
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            break;
        case cliqueSalvarArquivo:
            salvarArquivo(&estado_atual);
            break;
        case 404:
            estado_atual.currentPage = notFoundPage;
            break;
        default:
            break;
        }
    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP && currentPage->pageID == drawPage && estado_atual.lastButtonPressed->id == cliqueCriarPoligonos){
        handleCliqueCriarPoligonos(x, y, 1);
    }
}
/**
void drawMenu() {
    glutMouseFunc(mouseClickHandler);
    ProgramPage *current;

    switch(estado_atual.currentPage){
        case homePage:
            current = &paginaHome;
            break;
    }

    for (int i = 0; i < MAX_BUTTONS; i++){
        drawButton(&current->pageButtons[i]);
    }
}**/

void display(){

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

    //printf("Modo Exec = %d\n", estado_atual.modo_execucao);
    switch (estado_atual.currentPage){
    case homePage:
        showPage(&paginaHome);
        break;
    case drawPage:
        showPage(&drawingPage);
        if (showPontosPoliProgress){
            ListaPontos aux = *tempListPontosPoli;
            glColor3d(0, 0, 0);
            glBegin(GL_LINE_STRIP);
            while(aux != NULL){
                glVertex2d(aux->ponto.x, aux->ponto.y);
                aux = aux->proximo;
            }
            glEnd();
        }
        break;
    case notFoundPage:
        showPage(&errorPage);
        break;
    default:
        break;
    }

  glFlush(); //desenha os comandos não executados
}

void desenhasFormasGeometricas(){
    desenhaPoligonos(estado_atual.poligonos_criados);
    desenhaRetas(estado_atual.retas_criadas);
    desenhaPontos(estado_atual.pontos_criados);
}

void init(){
    estado_atual.currentPage = homePage;
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);

    int msgLength = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*) "404 ERROR: PAGE NOT FOUND");
    criar_pagina(&errorPage, notFoundPage, NULL);
    criar_pagina(&paginaHome, homePage, NULL);
    criar_pagina(&drawingPage, drawPage, &desenhasFormasGeometricas);
    add_text(&errorPage, "404 ERROR: PAGE NOT FOUND", GLUT_BITMAP_TIMES_ROMAN_24, (ortoSizeX - msgLength )/ 2, ortoSizeY/2);

    glClearColor(1, 1, 1, 1.0); //define a cor de fundo

    glMatrixMode(GL_PROJECTION); //carrega a matriz de projeção
    glOrtho(0, ortoSizeX, 0, ortoSizeY, -1.0, 1.0); //define a projeção ortogonal 2D

    double width = 150, height = 30;

    criarBotao(&paginaHome, 404, 50, ortoSizeY-130, width, height, "PAGINA INEXISTENTE", rgb);
    criarBotao(&paginaHome, cliqueNovoArquivo, 50, ortoSizeY-130-40, width, height, "NOVO ARQUIVO", rgb);
    criarBotao(&paginaHome, cliqueCarregarArquivo, 50, ortoSizeY-130-80, width, height, "CARREGAR ARQUIVO", rgb);

    width = 80;

    criarBotao(&drawingPage, cliqueCriarPontos,ortoSizeX-10-width, ortoSizeY-((height+10) * (drawingPage.qtd_buttons + 1)), width, height, "PONTO", rgb);
    criarBotao(&drawingPage, cliqueCriarRetas,ortoSizeX-10-width, ortoSizeY-((height+10) * (drawingPage.qtd_buttons + 1)), width, height, "RETA", rgb);
    criarBotao(&drawingPage, cliqueCriarPoligonos,ortoSizeX-10-width, ortoSizeY-((height+10) * (drawingPage.qtd_buttons + 1)), width, height, "POLI", rgb);
    criarBotao(&drawingPage, cliqueSalvarArquivo,ortoSizeX-10-width, ortoSizeY-((height+10) * (drawingPage.qtd_buttons + 1)), width, height, "SALVAR", rgb);
    criarBotao(&drawingPage, cliqueSelecionarobjeto,ortoSizeX-10-width, ortoSizeY-((height+10) * (drawingPage.qtd_buttons + 1)), width, height, "SELECT", rgb);
}


void mouseEnterButton(int x , int y){
    y = (ortoSizeY - y);
    ProgramPage *currentPage = getCurrentPage();

    for (int c = 0; c < currentPage->qtd_buttons; c++){

        if (isPointInsideButton(x, y, currentPage->pageButtons[c]) && currentPage->pageButtons[c].ativo == 0){
            printf("Mouse Enter\n");
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
            currentPage->pageButtons[c].ativo = 1;
            glutPostRedisplay();
            break;
        }
        if (!isPointInsideButton(x, y, currentPage->pageButtons[c]) && currentPage->pageButtons[c].ativo == 1){
            printf("Mouse out\n");
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            currentPage->pageButtons[c].ativo = 0;
            glutPostRedisplay();
            break;
        }
    }
}

/**
void exitCallback(){
    if (estado_atual.currentPage != drawPage)
        return;

     salvarArquivo(&estado_atual);
}
**/

int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowSizeX, windowSizeY);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Paint");

    glutMouseFunc(mouseClickHandler);
    glutPassiveMotionFunc(mouseEnterButton);
    glutKeyboardUpFunc(handleKeyboardUpIn);
    glutKeyboardFunc(handleKeyboardDownIn);
    //atexit(exitCallback);

    init();

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
