#ifndef LISTAPOLIGONOS_HPP_INCLUDED
#define LISTAPOLIGONOS_HPP_INCLUDED
#include "estruturas.hpp"

typedef struct elemento_poli *ListaPoligonos;
typedef struct poligono Poligono;

ListaPoligonos* criarListaPoligonos();
void destruirListaPoligonos(ListaPoligonos *);
int ListaPoligonosVazia(ListaPoligonos *);
int ListaPoligonosInserirFim (ListaPoligonos *, Poligono);
int ListaPoligonosRemoverFim (ListaPoligonos *);
int ListaPoligonosRemoverValor (ListaPoligonos *, int);
int ListaPoligonosAcessarIndice (ListaPoligonos *, int , Poligono *);
int ListaPoligonosAcessarValor (ListaPoligonos *, int , Poligono *);
int desenhaPoligonos(ListaPoligonos *ldse);
void salvarListaPoligonos(FILE *fp, ListaPoligonos *lista);
void carregarListaPoligonos(FILE *fp, EstadoExecucao *estado);
Poligono *pickPolygonIteration(ListaPoligonos *lista, int mouseX, int mouseY);

#endif // LISTAPOLIGONOS_HPP_INCLUDED
