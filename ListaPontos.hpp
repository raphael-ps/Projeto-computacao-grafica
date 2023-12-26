#ifndef LISTAPONTOS_HPP_INCLUDED
#define LISTAPONTOS_HPP_INCLUDED

typedef struct elemento* ListaPontos;
typedef struct ponto Ponto;

ListaPontos* criarListaPontos();
void destruirListaPontos(ListaPontos *);
int ListaPontosVazia(ListaPontos *);
int ListaPontosInserirFim (ListaPontos *, Ponto *);
int ListaPontosRemoverFim (ListaPontos *);
int ListaPontosRemoverValor (ListaPontos *, int);
int ListaPontosAcessarIndice (ListaPontos *, int , Ponto *);
int ListaPontosAcessarValor (ListaPontos *, int , Ponto *);
int desenhaPontos(ListaPontos *ldse);

#endif // LISTAPONTOS_HPP_INCLUDED
