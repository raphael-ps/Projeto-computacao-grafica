#ifndef LISTARETAS_HPP_INCLUDED
#define LISTARETAS_HPP_INCLUDED

typedef struct elemento_reta* ListaRetas;
typedef struct reta Reta;

ListaRetas* criarListaRetas();
void destruirListaRetas(ListaRetas *);
int ListaRetasVazia(ListaRetas *);
int ListaRetasInserirFim (ListaRetas *, Reta *);
int ListaRetasRemoverFim (ListaRetas *);
int ListaRetasRemoverValor (ListaRetas *, int);
int ListaRetasAcessarIndice (ListaRetas *, int , Reta *);
int ListaRetasAcessarValor (ListaRetas *, int , Reta *);
int desenhaRetas(ListaRetas *);


#endif // LISTARETAS_HPP_INCLUDED
