#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "myUtils.hpp"

void criarBotao(Button *novo_botao, int id, int x, int y, double width, double height, char *label, double rgb[3]){
    novo_botao->id = id;
    novo_botao->x = x;
    novo_botao->y = y;
    novo_botao->width = width;
    novo_botao->height = height;
    strcpy(novo_botao->label, label);
    for (int i = 0; i < 3; i++){
        novo_botao->rgb[i] = rgb[i];
    }
    novo_botao->ativo = 0;
}

int isPointInsideButton(int x, int y, Button button) {
    if (button.height == 0){
        return 0;
    }

    if ((x >= button.x && x <= button.x + button.width) && (y >= button.y && y <= button.y + button.height)){
        return 1;
    }
    return 0;
}
