// Para compilar en MacOSX
// g++ -o practica01 practica01.cpp -framework OpenGL -framework GLUT

#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#define NUMVTX (8)

static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=0.0f;

GLuint VertexArrayID;


/*GLfloat v[NUMVTX][3]={{-1,0,1},{-1,0,-1},
                      {0,-1,1},{0,-1,-1},
                      {1,1,1},{1,1,-1},
                      {-1,0,1},{-1,0,-1}};*/

GLfloat v[][3] = {
    {1,-2,3},{-1,-2,3},
    {1,-1,-1},{-1,-1,-1},
    {1,0,0},{-1,0,0},
    {1,1,-1},{-1,1,-1},
    {1,2,3},{-1,2,3}
        };

GLfloat c[NUMVTX][3]={{1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1}};
                      //{1,0,0},{1,0,0}};

void drawAxis(){
    glLineWidth(2.0);
    glColor3f(1.0f,0.0f,0.0f); //eje x

    float i = 5.0f;

    glBegin(GL_LINES);
        glVertex3f(-i,0.0f,0.0f);
        glVertex3f(i,0.0f,0.0f);
    glEnd();
    glColor3f(0.0f,1.0f,0.0f); //eje y
    glBegin(GL_LINES);
        glVertex3f(0.0f,-i,0.0f);
        glVertex3f(0.0f,i,0.0f);
    glEnd();
    glColor3f(0.0f,0.0f,1.0f); //eje z
    glBegin(GL_LINES);
        glVertex3f(0.0f,0.0f,-i);
        glVertex3f(0.0f,0.0f,i);
    glEnd();
}

void init(void){

}

void drawgraphix()
{
	int i;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	glTranslatef(0.0f,0.0f,-1.0f);

	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);


	glBegin(GL_QUAD_STRIP);
	for (i=0; i<10; i++)
	{
		//glColor3fv(c[i]);
		glVertex3fv(v[i]);
	}
	glEnd();
	drawAxis();

	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height)
{
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f);
}

void mousefunc(int button,int state,int x,int y)
{
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y)
{
	if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
		}

		mx=x;
		my=y;

		display();

		flag=2;
	}
}

void keyboardfunc(unsigned char key,int x,int y)
{
	if (key=='q' || key==27) exit(0);
}


void idlefunc()
{
	glutPostRedisplay();
}

int main(int argc,char **argv)
{
   winwidth=winheight=800;

   glutInit(&argc,argv);
   glutInitWindowSize(winwidth,winheight);
   glutInitWindowPosition(100,100);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   glutCreateWindow("Practica 1 - Ejercicio 1.1");

   glutDisplayFunc(drawgraphix);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyboardfunc);
   glutIdleFunc(idlefunc);

   glutMainLoop();

   return(0);
}
