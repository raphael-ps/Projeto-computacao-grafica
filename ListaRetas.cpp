#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "ListaRetas.hpp"
#include "estruturas.hpp"

struct elemento_reta{
    Reta reta;
    struct elemento_reta *proximo;
};

typedef struct elemento_reta ElementoReta;

ListaRetas* criarListaRetas(){
    ListaRetas *ldse;
    ldse = (ListaRetas *)malloc(sizeof(ListaRetas));
    if(ldse != NULL){
        *ldse = NULL;
    }
    return ldse;
}

void destruirListaRetas(ListaRetas *ldse){
    if(ldse != NULL){
        ElementoReta *aux;
        while(*ldse != NULL){
            aux = *ldse;
            *ldse = (*ldse)->proximo;
            free(aux);
        }
    }
}

int ListaRetasInserirFim(ListaRetas *ldse, Reta reta){
    if(ldse == NULL){
        return 0;
    }else{

        ElementoReta *novo = (ElementoReta*)malloc(sizeof(ElementoReta));
        if(novo == NULL){
            return 0;
        }else{
            novo->reta = reta;
            novo->proximo = NULL;
            if(ListaRetasVazia(ldse)){
                *ldse = novo;
            }else{
                ElementoReta *aux = *ldse;
                while(aux->proximo != NULL){
                    aux = aux->proximo;
                }
                aux->proximo = novo;
            }
            return 1;
        }
    }
}

int ListaRetasRemoverFim(ListaRetas *ldse){

    if(ListaRetasVazia(ldse)){
        return 0;
    }else if((*ldse)->proximo == NULL){
        ElementoReta *aux = *ldse;
        *ldse = aux->proximo;
        free(aux);
        return 1;
    }else{
        ElementoReta *ant = *ldse;
        ElementoReta *aux = ant->proximo;
        while(aux->proximo != NULL){
            ant = aux;
            aux = aux->proximo;
        }
        ant->proximo = aux->proximo;
        free(aux);
        return 1;
    }
}


int ListaRetasRemoverValor(ListaRetas *ldse, int id){
    if(ListaRetasVazia(ldse)){
        return 0;
    }else if((*ldse)->reta.id == id){
        ElementoReta *aux = *ldse;
        *ldse = aux->proximo;
        free(aux);
        return 1;
    }else{
        ElementoReta *ant = *ldse;
        ElementoReta *aux = ant->proximo;
        while(aux != NULL && aux->reta.id != id){
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

int ListaRetasAcessarIndice(ListaRetas *ldse, int pos, Reta *reta){
    if(ListaRetasVazia(ldse) || pos < 0){
        return 0;
    }else{
        int cont = 0;
        ElementoReta *aux = *ldse;
        while(aux != NULL && pos != cont){
            aux = aux->proximo;
            cont++;
        }
        //chegou ao fim da lista e nao achou
        if(aux == NULL){
            return 0;
        }else{
            *reta = aux->reta;
            return 1;
        }

    }
}

int ListaRetasAcessarValor(ListaRetas *ldse, int id, Reta *reta){
    if(ListaRetasVazia(ldse)){
        return 0;
    }else{
        ElementoReta *aux = *ldse;
        while(aux != NULL && aux->reta.id != id){
            aux = aux->proximo;
        }
        //chegou ao fim da lista e nao achou
        if(aux == NULL){
            return 0;
        }else{
            *reta = aux->reta;
            return 1;
        }
    }
}


int desenhaRetas(ListaRetas *ldse){
    if(ldse == NULL){
        return 0;
    }else{
        ElementoReta *aux = *ldse;

        glLineWidth(5.0);
        glBegin(GL_LINES);
        while(aux != NULL){
            glColor3dv(aux->reta.rgb_color);
            glVertex2d(aux->reta.ponto1.x, aux->reta.ponto1.y);
            glVertex2d(aux->reta.ponto2.x, aux->reta.ponto2.y);
            aux = aux->proximo;
        }
        glEnd();
        return 1;
    }
}

int ListaRetasVazia(ListaRetas *ldse){
    if(ldse == NULL){
        return 1;
    }else if(*ldse == NULL){
        return 1;
    }else{
        return 0;
    }
}

void salvarListaRetas(FILE *fp, ListaRetas *lista){
    ListaRetas primeiro = *lista;
    while (primeiro != NULL) {
        fwrite(&primeiro->reta, sizeof(Reta), 1, fp);
        primeiro = primeiro->proximo;
    }
}

void carregarListaRetas(FILE *fp, EstadoExecucao *estado){
    estado->retas_criadas = criarListaRetas();
    printf("qtd ret2 %d\n", estado->qtd_retas);

    for (int i = 0; i < estado->qtd_retas; i++) {
        Reta ret;
        fread(&ret, sizeof(Reta), 1, fp);
        ListaRetasInserirFim(estado->retas_criadas, ret);
    }
}
