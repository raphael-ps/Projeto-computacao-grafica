#include <stdio.h>
#include <stdlib.h>
#include "estruturas.hpp"
#include "ListaRetas.hpp"
#include "ListaPontos.hpp"
#include "ListaPoligonos.hpp"

#define TAM_INI 20


EstadoExecucao *criar_execucao(){
    static int id = 0;

    EstadoExecucao *novo_estado = (EstadoExecucao *) malloc(sizeof(EstadoExecucao));

    novo_estado->pontos_criados = criarListaPontos();
    novo_estado->retas_criadas = criarListaRetas();
    novo_estado->poligonos_criados = criarListaPoligonos();

    novo_estado->id = id;
    id++;

    return novo_estado;
}

Ponto *criar_ponto(EstadoExecucao *estado_atual, double coord_x, double coord_y, double rgb_vector[3]){
    static int id = 0;

    Ponto *novo_ponto = (Ponto *) malloc(sizeof(Ponto));

    novo_ponto->id = id;
    novo_ponto->x = coord_x;
    novo_ponto->y = coord_y;
    novo_ponto->rgb_color[0] = rgb_vector[0];
    novo_ponto->rgb_color[1] = rgb_vector[1];
    novo_ponto->rgb_color[2] = rgb_vector[2];

    if (ListaPontosInserirFim(estado_atual->pontos_criados, novo_ponto)){
        estado_atual->qtd_pontos++;
    }
    else{
        printf("Erro ao criar ponto.\n");
        return NULL;
    }

    id++;
    return novo_ponto;
}

Reta *criar_reta(EstadoExecucao *estado_atual, Ponto ponto1, Ponto ponto2, double rgb_vector[3]){
    static int id = 0;

    Reta *nova_reta = (Reta *) malloc(sizeof(Reta));

    nova_reta->ponto1 = ponto1;
    nova_reta->ponto2 = ponto2;
    nova_reta->id = id;
    nova_reta->rgb_color[0] = rgb_vector[0];
    nova_reta->rgb_color[1] = rgb_vector[1];
    nova_reta->rgb_color[2] = rgb_vector[2];

    if (ListaRetasInserirFim(estado_atual->retas_criadas, nova_reta)){
        estado_atual->qtd_retas++;
    }
    else{
        printf("Erro ao criar reta.\n");
        return NULL;
    }

    id++;
    return nova_reta;
}

Poligono *criar_poligono(EstadoExecucao *estado_atual, ListaPontos *pontos, int qtd_pon_pol, double rgb_vector[3]){
    static int id = 0;

    Poligono *novo_poligono = (Poligono *) malloc(sizeof(Poligono));

    novo_poligono->id = id;
    novo_poligono->pontos = pontos;
    novo_poligono->qtd_pontos = qtd_pon_pol;
    novo_poligono->rgb_color[0] = rgb_vector[0];
    novo_poligono->rgb_color[1] = rgb_vector[1];
    novo_poligono->rgb_color[2] = rgb_vector[2];

    if (ListaPoligonosInserirFim(estado_atual->poligonos_criados, novo_poligono)){
        estado_atual->qtd_poligonos++;
    }
    else{
        printf("Erro ao criar poligono.\n");
        return NULL;
    }

    id++;
    return novo_poligono;
}
