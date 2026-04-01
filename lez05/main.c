#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

/**
Scrivere un programma che disegni una stella a cinque punte. Utilizzando le
callback per la tastiera, la stella deve interattivamente ruotare su sé stessa,
cambiare dimensioni, e spostarsi all’interno della finestra. Notare la
composizione delle trasformazioni.
*/

GLvoid display();
GLvoid keyboard(unsigned char key, GLint x, GLint y);
GLvoid mouse(GLint button, GLint state, GLint x, GLint y);

const char *const WINDOW_TITLE = "Lezione 5 - Trasformazioni di Modellazione";
GLdouble translate[3] = {0.0, 0.0, 0.0};
GLdouble rotate[3] = {0.0, 0.0, 0.0};
GLdouble scale[3] = {1.0, 1.0, 1.0};

int main(int argc, char **argv) {
  const GLint size = 450;
  GLint window;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);

  glutInitWindowPosition(200, 200);
  glutInitWindowSize(size, size);
  window = glutCreateWindow(WINDOW_TITLE);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}

GLvoid display() {
  const GLint sides = 5;
  const Point center = {0.0, 0.0};
  const GLdouble r1 = 0.5;
  const GLdouble r2 = 0.25;
  const GLdouble rgb[] = {0.0, 0.0, 1.0};
  int i;

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(translate[0], translate[1], translate[2]);
  glRotatef(rotate[0], 1.0, 0.0, 0.0);
  glRotatef(rotate[1], 0.0, 1.0, 0.0);
  glRotatef(rotate[2], 0.0, 0.0, 1.0);
  glScalef(scale[0], scale[1], scale[2]);

  glPointSize(1.0);
  drawStar(&center, sides, r1, r2, rgb);

  checkErrors(WINDOW_TITLE);
  glFlush();
}

GLvoid keyboard(unsigned char key, GLint x, GLint y) {
  switch (key) {
  case 'd':
    translate[0] += 0.1;
    printf("translate[0]: %f\n", translate[0]);
    break;
  case 'a':
    translate[0] -= 0.1;
    printf("translate[0]: %f\n", translate[0]);
    break;
  case 'w':
    translate[1] += 0.1;
    printf("translate[1]: %f\n", translate[1]);
    break;
  case 's':
    translate[1] -= 0.1;
    printf("translate[1]: %f\n", translate[1]);
    break;
  case 'q':
    translate[2] += 0.1;
    printf("translate[2]: %f\n", translate[2]);
    break;
  case 'e':
    translate[2] -= 0.1;
    printf("translate[2]: %f\n", translate[2]);
    break;
  case 'x':
    scale[0] += 0.1;
    printf("scale[0]: %f\n", scale[0]);
    break;
  case 'X':
    scale[0] -= 0.1;
    printf("scale[0]: %f\n", scale[0]);
    break;
  case 'y':
    scale[1] += 0.1;
    printf("scale[1]: %f\n", scale[1]);
    break;
  case 'Y':
    scale[1] -= 0.1;
    printf("scale[1]: %f\n", scale[1]);
    break;
  case 'z':
    scale[2] += 0.1;
    printf("scale[2]: %f\n", scale[2]);
    break;
  case 'Z':
    scale[2] -= 0.1;
    printf("scale[2]: %f\n", scale[2]);
    break;
  case 'r':
    translate[0] = 0.0;
    translate[1] = 0.0;
    translate[2] = 0.0;
    rotate[0] = 0.0;
    rotate[1] = 0.0;
    rotate[2] = 0.0;
    scale[0] = 1.0;
    scale[1] = 1.0;
    scale[2] = 1.0;
    printf("reset\n");
    break;
  case 27: // ESC key
    exit(0);
  }
  glutPostRedisplay();
}

GLvoid mouse(GLint button, GLint state, GLint x, GLint y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      rotate[0] += 10.0;
      glutPostRedisplay();
      printf("rotate[0]: %f\n", rotate[0]);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      rotate[1] += 10.0;
      glutPostRedisplay();
      printf("rotate[1]: %f\n", rotate[1]);
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if (state == GLUT_DOWN) {
      rotate[2] += 10.0;
      glutPostRedisplay();
      printf("rotate[2]: %f\n", rotate[2]);
    }
    break;
  }
}
