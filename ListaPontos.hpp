#ifndef LISTAPONTOS_HPP_INCLUDED
#define LISTAPONTOS_HPP_INCLUDED
#include "estruturas.hpp"

struct elemento{
    Ponto ponto;
    struct elemento *proximo;
};

typedef struct elemento* ListaPontos;
typedef struct ponto Ponto;

ListaPontos* criarListaPontos();
void destruirListaPontos(ListaPontos *);
int ListaPontosVazia(ListaPontos *);
int ListaPontosInserirFim (ListaPontos *, Ponto );
int ListaPontosRemoverFim (ListaPontos *);
int ListaPontosRemoverValor (ListaPontos *, int);
int ListaPontosAcessarIndice (ListaPontos *, int , Ponto *);
int ListaPontosAcessarValor (ListaPontos *, int , Ponto *);
int desenhaPontos(ListaPontos *ldse);
void salvarListaPontos(FILE *fp, ListaPontos *lista);
void carregarListaPontos(FILE *fp, EstadoExecucao *estado);
Ponto *pickPontoIteration(ListaPontos *lista, int mouseX, int mouseY);

#endif // LISTAPONTOS_HPP_INCLUDED
