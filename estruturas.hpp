#ifndef ESTRUTURAS_HPP_INCLUDED
#define ESTRUTURAS_HPP_INCLUDED

typedef struct elemento* ListaPontos;
typedef struct elemento_reta* ListaRetas;
typedef struct elemento_poli* ListaPoligonos;

typedef struct ponto Ponto;
typedef struct reta Reta;
typedef struct poligono Poligono;

struct ponto{
    int id;
    double x;
    double y;
    double rgb_color[3];
};

struct reta{
    int id;
    Ponto ponto1;
    Ponto ponto2;
    double rgb_color[3];

};

struct poligono{
    int id;
    int qtd_pontos;
    ListaPontos *pontos;
    double rgb_color[3];

};

typedef struct{
    int id;

    int qtd_pontos;
    ListaPontos *pontos_criados;

    int qtd_retas;
    ListaRetas *retas_criadas;

    int qtd_poligonos;
    ListaPoligonos *poligonos_criados;

} EstadoExecucao;

EstadoExecucao *criar_execucao();
Ponto *criar_ponto(EstadoExecucao *estado_atual, double coord_x, double coord_y, double rgb_vector[3]);
Reta *criar_reta(EstadoExecucao *estado_atual, Ponto ponto1, Ponto ponto2, double rgb_vector[3]);
Poligono *criar_poligono(EstadoExecucao *estado_atual, ListaPontos *pontos, int qtd_pon_pol, double rgb_vector[3]);

#endif // ESTRUTURAS_HPP_INCLUDED
