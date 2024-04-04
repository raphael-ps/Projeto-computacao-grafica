#include <gl/glut.h>

GLfloat fAspect;
GLfloat rotationAngle = 0.0f;

void IdleFunc(void) //FUNÇAO PARA SOLICITAR "RE-DESENHO", APLICANDO ASSIM A ROTAÇAO
{
    glutPostRedisplay();
}

void drawText(char *texto, int x, int y, int z){  //DESENHAR TEXTOS NA TELA
    glRasterPos3d(x, y, z);
    for (int c = 0; c < (int) strlen(texto); c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto[c]);
    }
}

// CONFIGURAR AS FONTES DE LUZ
void setupLight(void) {
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat ambient_light[] = { 0.2, 0.2, 0.2, 1.0 }; // LUZ AMBIENTE
    GLfloat diffuse_light[] = { 0.7, 0.7, 0.7, 1.0 }; // LUZ DIFUSA
    GLfloat specular_light[] = { 1.0, 1.0, 1.0, 1.0 }; // LUZ ESPECULAR

    // CONFIGURAÇAO DE FATO
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    //ATIVAÇAO
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void Desenha(void)
{
    int altura = glutGet(GLUT_WINDOW_HEIGHT);
    int largura = glutGet(GLUT_WINDOW_WIDTH);
    fAspect = largura / altura;

    //glColor3f(0.5294f, 0.5216f, 0.5059f);
    glClearColor(0.7f,0.7f,0.7f,1);

    //
    float gold[13] = { 0.24725, 0.1995, 0.0745, 1.0,
                   0.75164, 0.60648, 0.22648, 1.0,
                   0.628281, 0.555802, 0.366065, 1.0,
                   50.0};

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, gold );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &gold[4] );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &gold[8] );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, gold[12] );

    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // PRIMEIRA VIEWPORT: PLANO XY
    glViewport(0, altura/2, largura/2, altura/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0); // VISTA DO PLANO XY
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); //ROTAÇAO
    glutSolidTeapot(50.0f);

    // SEGUNDA VIEWPORT: PLANO XZ
    glViewport(0, 0, largura/2, altura/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 50, 0, 0, 0, 0, 0, 0, -1); // VISTA DO PLANO XZ
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // ROTAÇAO
    glutSolidTeapot(50.0f);

    // TERCEIRA VIEWPORT: PLANO YZ
    glViewport(largura/2, altura/2, largura/2, altura/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(70, 0, 0, 0, 0, 0, 0, 1, 0); // VISTA DO PLANO YZ
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // ROTAÇAO
    glutSolidTeapot(50.0f);

    // QUARTA VIEWPORT: PERSPECTIVA
    glViewport(largura/2, 0, largura/2, altura/2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,fAspect,0.4,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(150, 150, 200, 0, 0, 0, 0, 1, 0); // VISTA EM PERSPECTIVA
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // ROTAÇAO
    glutSolidTeapot(50.0f);

    glDisable(GL_LIGHTING);

    // BORDAS DAS VIEWPORTS
    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, largura, 0, altura);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2i(largura/2, 0);
        glVertex2i(largura/2, altura);
        glVertex2i(0, altura/2);
        glVertex2i(largura, altura/2);
    glEnd();

    //TEXTOS DA TELA
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("XY", largura/2 - 35, altura/2 + 6, 0);
    drawText("XZ", largura/2 - 35, 6, 0);
    drawText("Perspectiva", largura - 105, 6, 0);
    drawText("YZ", largura - 35, altura/2 + 6, 0);

        glEnable(GL_LIGHTING);

    rotationAngle += 0.01f; // INCREMENTA O ANGULO DE ROTAÇAO

    glutSwapBuffers();
}

void Inicializa (void)
{
    glClearColor(0.96, 0.96, 0.96, 1);
    glEnable(GL_DEPTH_TEST);

    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;

	// HABILITA O MODELO DE COLORIZAÇAO DE GOURAUD
	glShadeModel(GL_SMOOTH);

	// DEFINE A CAPACIDADE DE REFLEXAO DO MATERIAL
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// DEFINE O BRILHO
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

    glEnable(GL_DEPTH_TEST);
    setupLight(); // CHAMA A FUNCAO PARA CONFIGURAR AS LUZES
}

/**
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400,400);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutIdleFunc(IdleFunc);
	Inicializa();
	glutMainLoop();
	return 0;
}
**/
