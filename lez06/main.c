#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

/**
Creare un prisma regolare con base ottagonale avente
il centro della base inferiore in (0.2, 0.2, -0.5),
il centro della base superiore in (0.2, 0.2, -1.5)
e basi circoscritte dalla circonferenza di raggio 0.5

Utilizzando in maniera combinata le callback per mouse e tastiera,
rendere modificabili i tre valori di altezza larghezza e profondità
del Volume di Vista prendendo come valori iniziali quelli di:
glOrtho(-1, 1, -1, 1, 1, 5)
*/

GLvoid display();
GLvoid setProjection();
GLvoid keyboard(unsigned char key, GLint x, GLint y);
GLvoid mouse(GLint button, GLint state, GLint x, GLint y);
GLvoid motion(GLint x, GLint y);
GLvoid gluPerspectiveWrapper(GLdouble left, GLdouble right, GLdouble bottom,
                             GLdouble top, GLdouble near_plane,
                             GLdouble far_plane);

const char *const WINDOW_TITLE = "Lezione 6 - Trasformazioni di Proiezione";
GLdouble view[6] = {-1.0, 1.0, -1.0, 1.0, 1.0, 5.0};
GLdouble translate[3] = {0.0, 0.0, -3.0};
GLdouble rotate[3] = {0.0, 0.0, 0.0};
GLdouble scale[3] = {1.0, 1.0, 1.0};
void (*projection_func)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble,
                        GLdouble) = glOrtho;

const double step = 0.1;
const double sensitivity = 0.5;
int leftMouseDown = 0;
int rightMouseDown = 0;
int lastMouseX = 0;
int lastMouseY = 0;

int main(int argc, char **argv) {
  const GLint size = 450;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glEnable(GL_DEPTH_TEST);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(size, size);
  (void)glutCreateWindow(WINDOW_TITLE);

  setProjection();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}

GLvoid setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  projection_func(view[LEFT], view[RIGHT], view[BOTTOM], view[TOP],
                  view[NEAR_PLANE], view[FAR_PLANE]);
}

GLvoid display() {
  const GLint sides = 8;
  const Point lower_base_center = {0.2, 0.2, -0.5};
  const Point upper_base_center = {0.2, 0.2, -1.5};
  const GLdouble radius = 0.5;
  Point points[4];
  GLdouble angle, x, y, z;
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(translate[X], translate[Y], translate[Z]);
  glRotatef(rotate[X], 1.0, 0.0, 0.0);
  glRotatef(rotate[Y], 0.0, 1.0, 0.0);
  glRotatef(rotate[Z], 0.0, 0.0, 1.0);
  glScalef(scale[X], scale[Y], scale[Z]);

  glPointSize(1.0);
  glColor3fv(blue);
  drawPrism(sides, lower_base_center, upper_base_center, radius);

  checkErrors(WINDOW_TITLE);
  glFlush();
}

GLvoid keyboard(unsigned char key, GLint x, GLint y) {
  switch (key) {
  case 'w':
    view[TOP] += step;
    break;
  case 'W':
    view[TOP] -= step;
    break;
  case 's':
    view[BOTTOM] += step;
    break;
  case 'S':
    view[BOTTOM] -= step;
    break;
  case 'a':
    view[LEFT] += step;
    break;
  case 'A':
    view[LEFT] -= step;
    break;
  case 'd':
    view[RIGHT] += step;
    break;
  case 'D':
    view[RIGHT] -= step;
    break;
  case 'q':
    view[NEAR_PLANE] += step;
    break;
  case 'Q':
    view[NEAR_PLANE] -= step;
    break;
  case 'e':
    view[FAR_PLANE] += step;
    break;
  case 'E':
    view[FAR_PLANE] -= step;
    break;
  case 'r':
    view[LEFT] = -1.0;
    view[RIGHT] = 1.0;
    view[BOTTOM] = -1.0;
    view[TOP] = 1.0;
    view[NEAR_PLANE] = 1.0;
    view[FAR_PLANE] = 5.0;
    translate[X] = 0.0;
    translate[Y] = 0.0;
    translate[Z] = -3.0;
    rotate[X] = 0.0;
    rotate[Y] = 0.0;
    rotate[Z] = 0.0;
    scale[X] = 1.0;
    scale[Y] = 1.0;
    scale[Z] = 1.0;
    break;
  case '1':
    printf("Projection: glOrtho\n");
    projection_func = glOrtho;
    break;
  case '2':
    printf("Projection: glFrustum\n");
    projection_func = glFrustum;
    break;
  case '3':
    printf("Projection: gluPerspective\n");
    projection_func = gluPerspectiveWrapper;
    break;
  case 27: // ESC key
    exit(0);
  }
  setProjection();
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
  setProjection();
  glutPostRedisplay();
}

GLvoid gluPerspectiveWrapper(GLdouble left, GLdouble right, GLdouble bottom,
                             GLdouble top, GLdouble near_plane,
                             GLdouble far_plane) {
  gluPerspective(45.0, (right - left) / (top - bottom), near_plane, far_plane);
}
