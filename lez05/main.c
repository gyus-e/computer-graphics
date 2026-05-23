#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
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
GLvoid motion(GLint x, GLint y);

const char *const WINDOW_TITLE = "Lezione 5 - Trasformazioni di Modellazione";
GLdouble translate[3] = {0.0, 0.0, 0.0};
GLdouble rotate[3] = {0.0, 0.0, 0.0};
GLdouble scale[3] = {1.0, 1.0, 1.0};

const double step = 0.1;
const double sensitivity = 0.5;
int leftMouseDown = 0;
int rightMouseDown = 0;
int lastMouseX = 0;
int lastMouseY = 0;

int main(int argc, char **argv) {
  const GLint size = 450;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);

  glutInitWindowPosition(200, 200);
  glutInitWindowSize(size, size);
  (void)glutCreateWindow(WINDOW_TITLE);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}

GLvoid display() {
  const GLint sides = 5;
  const Point center = {0.0, 0.0};
  const GLdouble r1 = 0.5;
  const GLdouble r2 = 0.25;
  const GLdouble blue[] = {0.0, 0.0, 1.0};
  int i;

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(translate[X], translate[Y], translate[Z]);
  glRotatef(rotate[X], 1.0, 0.0, 0.0);
  glRotatef(rotate[Y], 0.0, 1.0, 0.0);
  glRotatef(rotate[Z], 0.0, 0.0, 1.0);
  glScalef(scale[X], scale[Y], scale[Z]);

  glPointSize(1.0);
  glColor3dv(blue);
  drawStar(center, sides, r1, r2);
  checkErrors(WINDOW_TITLE);
  glFlush();
}

GLvoid keyboard(unsigned char key, GLint x, GLint y) {
  switch (key) {
  case 'd':
    translate[X] += step;
    break;
  case 'a':
    translate[X] -= step;
    break;
  case 'w':
    translate[Y] += step;
    break;
  case 's':
    translate[Y] -= step;
    break;
  case '+':
    translate[Z] += step;
    break;
  case '-':
    translate[Z] -= step;
    break;
  case 'x':
    scale[X] += step;
    break;
  case 'X':
    scale[X] -= step;
    break;
  case 'y':
    scale[Y] += step;
    break;
  case 'Y':
    scale[Y] -= step;
    break;
  case 'z':
    scale[Z] += step;
    break;
  case 'Z':
    scale[Z] -= step;
    break;
  case 'r':
    translate[X] = 0.0;
    translate[Y] = 0.0;
    translate[Z] = 0.0;
    rotate[X] = 0.0;
    rotate[Y] = 0.0;
    rotate[Z] = 0.0;
    scale[X] = 1.0;
    scale[Y] = 1.0;
    scale[Z] = 1.0;
    break;
  case 27: // ESC key
    exit(0);
  }
  glutPostRedisplay();
}

GLvoid mouse(GLint button, GLint state, GLint x, GLint y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    leftMouseDown = (state == GLUT_DOWN);
  case GLUT_RIGHT_BUTTON:
    rightMouseDown = (state == GLUT_DOWN);
  default:
    lastMouseX = x;
    lastMouseY = y;
  }
}

GLvoid motion(GLint x, GLint y) {
  int dx = x - lastMouseX;
  int dy = y - lastMouseY;
  lastMouseX = x;
  lastMouseY = y;
  if (leftMouseDown) { 
    rotate[X] += dy * sensitivity;
    rotate[Y] += dx * sensitivity;
  }
  if (rightMouseDown) {
    rotate[Z] += dx * sensitivity;
  }
  glutPostRedisplay();
}
