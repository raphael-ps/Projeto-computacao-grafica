#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "ListaPoligonos.hpp"
#include "ListaPontos.hpp"
#include "estruturas.hpp"

struct elemento_poli{
    Poligono poligono;
    struct elemento_poli *proximo;
};

typedef struct elemento_poli ElementoPoli;

ListaPoligonos* criarListaPoligonos(){
    ListaPoligonos *ldse;
    ldse = (ListaPoligonos *)malloc(sizeof(ListaPoligonos));
    if(ldse != NULL){
        *ldse = NULL;
    }
    return ldse;
}

void destruirListaPoligonos(ListaPoligonos *ldse){
    if(ldse != NULL){
        ElementoPoli *aux;
        while(*ldse != NULL){
            aux = *ldse;
            *ldse = (*ldse)->proximo;
            free(aux);
        }
    }
}

int ListaPoligonosInserirFim(ListaPoligonos *ldse, Poligono poligono){
    if(ldse == NULL){
        return 0;
    }else{

        ElementoPoli *novo = (ElementoPoli*)malloc(sizeof(ElementoPoli));
        if(novo == NULL){
            return 0;
        }else{
            novo->poligono = poligono;
            novo->proximo = NULL;
            if(ListaPoligonosVazia(ldse)){
                *ldse = novo;
            }else{
                ElementoPoli *aux = *ldse;
                while(aux->proximo != NULL){
                    aux = aux->proximo;
                }
                aux->proximo = novo;
            }
            return 1;
        }
    }
}

int ListaPoligonosRemoverFim(ListaPoligonos *ldse){

    if(ListaPoligonosVazia(ldse)){
        return 0;
    }else if((*ldse)->proximo == NULL){
        ElementoPoli *aux = *ldse;
        *ldse = aux->proximo;
        free(aux);
        return 1;
    }else{
        ElementoPoli *ant = *ldse;
        ElementoPoli *aux = ant->proximo;
        while(aux->proximo != NULL){
            ant = aux;
            aux = aux->proximo;
        }
        ant->proximo = aux->proximo;
        free(aux);
        return 1;
    }
}


int ListaPoligonosRemoverValor(ListaPoligonos *ldse, int id){
    if(ListaPoligonosVazia(ldse)){
        return 0;
    }else if((*ldse)->poligono.id == id){
        ElementoPoli *aux = *ldse;
        *ldse = aux->proximo;
        free(aux);
        return 1;
    }else{
        ElementoPoli *ant = *ldse;
        ElementoPoli *aux = ant->proximo;
        while(aux != NULL && aux->poligono.id != id){
            ant = aux;
            aux = aux->proximo;
        }
        //chegou ao fim da lista e nao achou o elemento
        if(aux == NULL){
            return 0;
        }
        //achou o elemento
        ant->proximo = aux->proximo;
        free(aux);
        return 1;
    }
}

int ListaPoligonosAcessarIndice(ListaPoligonos *ldse, int pos, Poligono *poligono){
    if(ListaPoligonosVazia(ldse) || pos < 0){
        return 0;
    }else{
        int cont = 0;
        ElementoPoli *aux = *ldse;
        while(aux != NULL && pos != cont){
            aux = aux->proximo;
            cont++;
        }
        //chegou ao fim da lista e nao achou
        if(aux == NULL){
            return 0;
        }else{
            *poligono = aux->poligono;
            return 1;
        }

    }
}

int ListaPoligonosAcessarValor(ListaPoligonos *ldse, int id, Poligono *poligono){
    if(ListaPoligonosVazia(ldse)){
        return 0;
    }else{
        ElementoPoli *aux = *ldse;
        while(aux != NULL && aux->poligono.id != id){
            aux = aux->proximo;
        }
        //chegou ao fim da lista e nao achou
        if(aux == NULL){
            return 0;
        }else{
            *poligono = aux->poligono;
            return 1;
        }
    }
}


int desenhaPoligonos(ListaPoligonos *ldse){
    if(ldse == NULL){
        return 0;
    }else{
        ElementoPoli *aux = *ldse;
        double rgb4selected[] = {1, 0.647, 0};
        glLineWidth(5.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        while(aux != NULL){
            ListaPontos *pol_pontos_lista = aux->poligono.pontos;
            struct elemento *pol_pontos = *pol_pontos_lista;

            glColor3dv(aux->poligono.selected ? rgb4selected : aux->poligono.rgb_color);
            printf("Poly id = %d\n", aux->poligono.id);
            glBegin(GL_POLYGON);
            while(pol_pontos != NULL){
                glVertex2d(pol_pontos->ponto.x, pol_pontos->ponto.y);
                pol_pontos = pol_pontos->proximo;
            }

            glEnd();

            aux = aux->proximo;
        }
        return 1;
    }
}

int ListaPoligonosVazia(ListaPoligonos *ldse){
    if(ldse == NULL){
        return 1;
    }else if(*ldse == NULL){
        return 1;
    }else{
        return 0;
    }
}

void salvarListaPoligonos(FILE *fp, ListaPoligonos *lista){
    ListaPoligonos primeiro = *lista;
    while (primeiro != NULL) {
        primeiro->poligono.selected = 0;
        fwrite(&primeiro->poligono, sizeof(Poligono), 1, fp);
        salvarListaPontos(fp, primeiro->poligono.pontos);
        primeiro = primeiro->proximo;
    }
}

void carregarListaPoligonos(FILE *fp, EstadoExecucao *estado){
    estado->poligonos_criados = criarListaPoligonos();
    printf("qtd pont2 %d\n", estado->qtd_poligonos);

    for (int i = 0; i < estado->qtd_poligonos; i++) {
        Poligono poli;
        fread(&poli, sizeof(Poligono), 1, fp);

        poli.pontos = criarListaPontos();
        for (int i = 0; i < poli.qtd_pontos; i++) {
            Ponto pon;
            fread(&pon, sizeof(Ponto), 1, fp);
            ListaPontosInserirFim(poli.pontos, pon);
        }
        ListaPoligonosInserirFim(estado->poligonos_criados, poli);
    }
}


Poligono *pickPolygonTest(Poligono *poly, int mouseX, int mouseY){
    int cont = 0;

    ListaPontos listNode = *poly->pontos;
    Ponto *firstPonto = &listNode->ponto;
    while (listNode != NULL){

        Ponto *arestaP1 = &listNode->ponto;
        Ponto *arestaP2 = listNode->proximo == NULL ? firstPonto : &listNode->proximo->ponto;

        if ((mouseY < arestaP1->y) != (mouseY < arestaP2->y) &&
            mouseX < (arestaP1->x + ((mouseY-arestaP1->y)/(arestaP2->y-arestaP1->y))*(arestaP2->x-arestaP1->x))){

            cont++;
        }

        listNode = listNode->proximo;
    }
    printf("%s - mouse: (%d, %d)[][][][]\n", cont % 2 == 1 ? "Dentro" : "Fora", mouseX, mouseY);
    if(cont % 2 == 1){
        poly->selected = 1;
        return poly;
    }

    return NULL;
}

Poligono *pickPolygonIteration(ListaPoligonos *lista, int mouseX, int mouseY){
    ListaPoligonos listNode = *lista;
    Poligono *selected;

    while (listNode != NULL){
        selected = pickPolygonTest(&listNode->poligono, mouseX, mouseY);

        if (selected != NULL){
            return selected;
        }

        listNode = listNode->proximo;
    }
    return NULL;
}
