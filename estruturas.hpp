#ifndef ESTRUTURAS_HPP_INCLUDED
#define ESTRUTURAS_HPP_INCLUDED

#define MAX_BUTTONS 10
#define MAX_TEXTS 10
#define MAX_CARACTERE 50
#define CLICK_TOLERANCE 10

enum positions{
    TOP = 1,
    LEFT = 8,
    RIGHT = 4,
    BOTTOM = 2,
    INSIDE = 0
};

enum tiposCliques{
    cliqueNovoArquivo = 1,
    cliqueCarregarArquivo,
    cliqueCriarPontos,
    cliqueCriarRetas,
    cliqueCriarPoligonos,
    cliqueSalvarArquivo,
    cliqueSelecionarobjeto
};

enum programPages {
    homePage = 50,
    drawPage,
    notFoundPage
};

typedef struct botao Button;
typedef struct elemento* ListaPontos;
typedef struct elemento_reta* ListaRetas;
typedef struct elemento_poli* ListaPoligonos;

typedef struct ponto Ponto;
typedef struct reta Reta;
typedef struct poligono Poligono;
typedef struct texto Text;

struct texto{
    char texto[MAX_CARACTERE];
    double xStart;
    double yStart;
    void *font;
};

struct botao{
    int id;
    int x;
    int y;
    double width;
    double height;
    Text label;
    double rgb[3];
    int ativo;
    int pressed;
};

typedef struct {
    int pageID;
    void (*showPageFunction)(void);
    Button pageButtons[MAX_BUTTONS];
    Text pageTexts[MAX_TEXTS];
    int qtd_buttons;
    int qtd_texts;
} ProgramPage;

struct ponto{
    int id;
    double x;
    double y;
    double rgb_color[3];
    int selected;
};

struct reta{
    int id;
    Ponto ponto1;
    Ponto ponto2;
    Ponto centroide;
    double rgb_color[3];
    int selected;
};

struct poligono{
    int id;
    int qtd_pontos;
    ListaPontos *pontos;
    Ponto centroide;
    double rgb_color[3];
    int selected;
};

typedef struct{
    int id;

    int lastObjectId;

    int qtd_pontos;
    ListaPontos *pontos_criados;

    int qtd_retas;
    ListaRetas *retas_criadas;

    int qtd_poligonos;
    ListaPoligonos *poligonos_criados;

    int currentPage;

    Ponto *lastPointSelected;
    Reta *lastLineSelected;
    Poligono *lastPolygonSelected;

    Button *lastButtonPressed;
} EstadoExecucao;


int isPointInsideButton(int x, int y, Button button);
//void criarBotao(ProgramPage *pg, int id, int x, int y, double width, double height, char *label, double rgb[3]);
void criarBotao(ProgramPage *pg, int id, int x, int y, double width, double height, char *label, double rgb[3]);
//EstadoExecucao *criar_execucao();
EstadoExecucao *criar_execucao(EstadoExecucao *novo_estado);
//Ponto *criar_ponto(EstadoExecucao *estado_atual, double coord_x, double coord_y, double rgb_vector[3]);
int criar_ponto(EstadoExecucao *estado_atual, double coord_x, double coord_y, double rgb_vector[3]);
int criar_reta(EstadoExecucao *estado_atual, Ponto ponto1, Ponto ponto2, double rgb_vector[3]);
int criar_poligono(EstadoExecucao *estado_atual, ListaPontos *pontos, Ponto centroide, int qtd_pon_pol, double rgb_vector[3]);
int criar_text(Text *destino, char *msg, void *font, double xStart, double yStart);
void drawText(Text texto);
int criar_pagina(ProgramPage *destino, int pageID, void (*pageFunction)(void));
void showPage(ProgramPage *pagina);
void drawButton(Button *button);
int add_text(ProgramPage *pg, char *msg, void *font, double xStart, double yStart);
int associar_ponto(Ponto *destino, double x, double y, double rgb[3]);

#endif // ESTRUTURAS_HPP_INCLUDED
