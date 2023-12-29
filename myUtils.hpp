#ifndef MYUTILS_HPP_INCLUDED
#define MYUTILS_HPP_INCLUDED

#define LABEL_MAX_LEN 100

typedef struct{
    int id;
    int x;
    int y;
    double width;
    double height;
    char label[LABEL_MAX_LEN];
    double rgb[3];
    int ativo;
} Button;

int isPointInsideButton(int x, int y, Button button);
void criarBotao(Button *novo_botao, int id, int x, int y, double width, double height, char *label, double rgb[3]);

#endif // MYUTILS_HPP_INCLUDED
