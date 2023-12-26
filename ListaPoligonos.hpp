#ifndef LISTAPOLIGONOS_HPP_INCLUDED
#define LISTAPOLIGONOS_HPP_INCLUDED

typedef struct elemento_poli *ListaPoligonos;
typedef struct poligono Poligono;

ListaPoligonos* criarListaPoligonos();
void destruirListaPoligonos(ListaPoligonos *);
int ListaPoligonosVazia(ListaPoligonos *);
int ListaPoligonosInserirFim (ListaPoligonos *, Poligono *);
int ListaPoligonosRemoverFim (ListaPoligonos *);
int ListaPoligonosRemoverValor (ListaPoligonos *, int);
int ListaPoligonosAcessarIndice (ListaPoligonos *, int , Poligono *);
int ListaPoligonosAcessarValor (ListaPoligonos *, int , Poligono *);
int desenhaPoligonos(ListaPoligonos *ldse);

#endif // LISTAPOLIGONOS_HPP_INCLUDED
