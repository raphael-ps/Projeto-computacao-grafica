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
        double rgb4selected[] = {1, 0.647, 0};
        glLineWidth(5.0);
        glBegin(GL_LINES);

        while(aux != NULL){
            glColor3dv(aux->reta.selected ? rgb4selected : aux->reta.rgb_color);
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
        primeiro->reta.selected = 0;
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

int encode(Ponto *point, int winXmin, int winYmin, int winXmax, int winYmax) {
    int code = 0;

  if (point->x < winXmin) {
    code |= 8;
  } else if (point->x > winXmax) {
    code |= 4;
  }

  if (point->y < winYmin ) {
    code |= 2;
  } else if (point->y > winYmax ) {
    code |= 1;
  }

  return code;
}

Reta *pickLineTest(Reta *line, int mouseX, int mouseY){

    int xmin = mouseX-CLICK_TOLERANCE, ymin = mouseY-CLICK_TOLERANCE;
    int xmax = mouseX+CLICK_TOLERANCE, ymax = mouseY+CLICK_TOLERANCE;

    int code1 = encode(&line->ponto1, xmin, ymin, xmax, ymax);
    int code2 = encode(&line->ponto2, xmin, ymin, xmax, ymax);

    Ponto tempP1 = {0, line->ponto1.x, line->ponto1.y, 0, 0};
    Ponto tempP2 = {0, line->ponto2.x, line->ponto2.y, 0, 0};


    do {

        int outcodeOut = code1;
        int c1_and_c2 = code1 & code2;

        if (code1 == INSIDE || code2 == INSIDE){

            line->selected = 1;
            return line;
        }

        if((c1_and_c2 & TOP) || (c1_and_c2 & BOTTOM) || (c1_and_c2 & RIGHT) || (c1_and_c2 & LEFT)){ //(outcode1 & outcode2)
            return NULL;
        }

        double x, y;

        if (outcodeOut & TOP) {
            x = tempP1.x + (tempP2.x - tempP1.x) * (ymax - tempP1.y) / (tempP2.y - tempP1.y);
            y = ymax;
        } else if (outcodeOut & BOTTOM) {
            x = tempP1.x + (tempP2.x - tempP1.x) * (ymin - tempP1.y) / (tempP2.y - tempP1.y);
            y = ymin;
        } else if (outcodeOut & RIGHT) {
            y = tempP1.y + (tempP2.y - tempP1.y) * (xmax - tempP1.x) / (tempP2.x - tempP1.x);
            x = xmax;
        } else if (outcodeOut & LEFT) {
            y = tempP1.y + (tempP2.y - tempP1.y) * (xmin - tempP1.x) / (tempP2.x - tempP1.x);
            x = xmin;
        }
       // printf("intersecao(%f, %f) janela = (%d, %d) (%d, %d)\n", x, y, xmin, ymin, xmax, ymax);

        tempP1.x = x;
        tempP1.y = y;

        code1 = encode(&tempP1, xmin, ymin, xmax, ymax);
        code2 = encode(&tempP2, xmin, ymin, xmax, ymax);
        outcodeOut = code1;
    } while (1);

    printf("Ponto1 code = %d\n", code1);
    printf("Ponto2 code = %d\n", code2);
    printf("AND OPERATION = %d\n", code1 & code2);
    printf("================\n\n");

    return NULL;
}


Reta *pickLineIteration(ListaRetas *lista, int mouseX, int mouseY){
    ListaRetas listNode = *lista;
    Reta *selected;

    while (listNode != NULL){
        selected = pickLineTest(&listNode->reta, mouseX, mouseY);

        if (selected != NULL){
            return selected;
        }

        listNode = listNode->proximo;
    }
    return NULL;
}
