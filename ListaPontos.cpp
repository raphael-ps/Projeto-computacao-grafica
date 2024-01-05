#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "ListaPontos.hpp"
#include "estruturas.hpp"


typedef struct elemento Elemento;

ListaPontos* criarListaPontos(){
    ListaPontos *ldse;
    ldse = (ListaPontos *)malloc(sizeof(ListaPontos));
    if(ldse != NULL){
        *ldse = NULL;
    }
    return ldse;
}

void destruirListaPontos(ListaPontos *ldse){
    if(ldse != NULL){
        Elemento *aux;
        while(*ldse != NULL){
            aux = *ldse;
            *ldse = (*ldse)->proximo;
            free(aux);
        }
    }
}

int ListaPontosInserirFim(ListaPontos *ldse, Ponto ponto){
    if(ldse == NULL){
        return 0;
    }else{

        Elemento *novo = (Elemento*)malloc(sizeof(Elemento));
        if(novo == NULL){
            return 0;
        }else{
            novo->ponto = ponto;
            novo->proximo = NULL;
            if(ListaPontosVazia(ldse)){
                *ldse = novo;
            }else{
                Elemento *aux = *ldse;
                while(aux->proximo != NULL){
                    aux = aux->proximo;
                }
                aux->proximo = novo;
            }
            return 1;
        }
    }
}

int ListaPontosRemoverFim(ListaPontos *ldse){

    if(ListaPontosVazia(ldse)){
        return 0;
    }else if((*ldse)->proximo == NULL){
        Elemento *aux = *ldse;
        *ldse = aux->proximo;
        free(aux);
        return 1;
    }else{
        Elemento *ant = *ldse;
        Elemento *aux = ant->proximo;
        while(aux->proximo != NULL){
            ant = aux;
            aux = aux->proximo;
        }
        ant->proximo = aux->proximo;
        free(aux);
        return 1;
    }
}


int ListaPontosRemoverValor(ListaPontos *ldse, int id){
    if(ListaPontosVazia(ldse)){
        return 0;
    }else if((*ldse)->ponto.id == id){
        Elemento *aux = *ldse;
        *ldse = aux->proximo;
        free(aux);
        return 1;
    }else{
        Elemento *ant = *ldse;
        Elemento *aux = ant->proximo;
        while(aux != NULL && aux->ponto.id != id){
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

int ListaPontosAcessarIndice(ListaPontos *ldse, int pos, Ponto *ponto){
    if(ListaPontosVazia(ldse) || pos < 0){
        return 0;
    }else{
        int cont = 0;
        Elemento *aux = *ldse;
        while(aux != NULL && pos != cont){
            aux = aux->proximo;
            cont++;
        }
        //chegou ao fim da lista e nao achou
        if(aux == NULL){
            return 0;
        }else{
            *ponto = aux->ponto;
            return 1;
        }

    }
}

int ListaPontosAcessarValor(ListaPontos *ldse, int id, Ponto *ponto){
    if(ListaPontosVazia(ldse)){
        return 0;
    }else{
        Elemento *aux = *ldse;
        while(aux != NULL && aux->ponto.id != id){
            aux = aux->proximo;
        }
        //chegou ao fim da lista e nao achou
        if(aux == NULL){
            return 0;
        }else{
            *ponto = aux->ponto;
            return 1;
        }
    }
}

int ListaPontosVazia(ListaPontos *ldse){
    if(ldse == NULL){
        return 1;
    }else if(*ldse == NULL){
        return 1;
    }else{
        return 0;
    }
}

int desenhaPontos(ListaPontos *ldse){
    if(ldse == NULL){
        return 0;
    }else{
        glPointSize(5.0);
        glBegin(GL_POINTS);
        double rgb4selected[] = {1, 0.647, 0};
        Elemento *aux2 = *ldse;
        while(aux2 != NULL){
            glColor3dv(aux2->ponto.selected ? rgb4selected : aux2->ponto.rgb_color);
            glVertex2d(aux2->ponto.x, aux2->ponto.y); //define localização do vértice
            aux2 = aux2->proximo;
        }
        glEnd();

        return 1;
    }
}

void salvarListaPontos(FILE *fp, ListaPontos *lista){
    ListaPontos primeiro = *lista;
    while (primeiro != NULL) {
        primeiro->ponto.selected = 0;
        fwrite(&primeiro->ponto, sizeof(Ponto), 1, fp);
        primeiro = primeiro->proximo;
    }
}

void carregarListaPontos(FILE *fp, EstadoExecucao *estado){
    estado->pontos_criados = criarListaPontos();
    printf("qtd pont2 %d\n", estado->qtd_pontos);

    for (int i = 0; i < estado->qtd_pontos; i++) {
        Ponto p;
        fread(&p, sizeof(Ponto), 1, fp);
        ListaPontosInserirFim(estado->pontos_criados, p);
    }
}


Ponto* pickPontoTest(Ponto *p, double mx, double my){

    if (mx <= p->x+CLICK_TOLERANCE && mx >= p->x-CLICK_TOLERANCE){
        if (my <= p->y+CLICK_TOLERANCE && my >= p->y-CLICK_TOLERANCE){
            p->selected = 1;
            return p;
        }
    }
    return NULL;
}

Ponto *pickPontoIteration(ListaPontos *lista, int mouseX, int mouseY){
    ListaPontos listNode = *lista;
    Ponto *selected;

    while (listNode != NULL){
        selected = pickPontoTest(&listNode->ponto, mouseX, mouseY);

        if (selected != NULL){
            return selected;
        }

        listNode = listNode->proximo;
    }
    return NULL;
}
