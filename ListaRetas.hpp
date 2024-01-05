#ifndef LISTARETAS_HPP_INCLUDED
#define LISTARETAS_HPP_INCLUDED
#include "estruturas.hpp"

typedef struct elemento_reta* ListaRetas;
typedef struct reta Reta;

ListaRetas* criarListaRetas();
void destruirListaRetas(ListaRetas *);
int ListaRetasVazia(ListaRetas *);
int ListaRetasInserirFim (ListaRetas *, Reta );
int ListaRetasRemoverFim (ListaRetas *);
int ListaRetasRemoverValor (ListaRetas *, int);
int ListaRetasAcessarIndice (ListaRetas *, int , Reta *);
int ListaRetasAcessarValor (ListaRetas *, int , Reta *);
int desenhaRetas(ListaRetas *);
void salvarListaRetas(FILE *fp, ListaRetas *lista);
void carregarListaRetas(FILE *fp, EstadoExecucao *estado);
Reta *pickLineIteration(ListaRetas *lista, int mouseX, int mouseY);

#endif // LISTARETAS_HPP_INCLUDED
