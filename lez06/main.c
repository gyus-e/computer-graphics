#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
Creare un prisma regolare con base ottagonale avente
il centro della base inferiore in (0.2, 0.2, -0.5),
il centro della base superiore in (0.2, 0.2, -1.5)
e basi circoscritte dalla circonferenza di raggio 0.5

Utilizzando in maniera combinata le callback per mouse e tastiera,
rendere modificabili i tre valori di altezza larghezza e profondità
del Volume di Visa prendendo come valori iniziali quelli di:
glOrtho(-1, 1, -1, 1, 1, 5)
*/

GLvoid display();
GLvoid setProjection();
GLvoid keyboard(unsigned char key, GLint x, GLint y);
GLvoid mouse(GLint button, GLint state, GLint x, GLint y);
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

int main(int argc, char **argv) {
  const GLint size = 450;
  GLint window;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glEnable(GL_DEPTH_TEST);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(size, size);
  window = glutCreateWindow(WINDOW_TITLE);

  setProjection();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
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
  const GLdouble rgb[] = {0.0, 0.0, 1.0};
  Point points[4];
  GLdouble angle, x, y, z;
  int i;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(translate[0], translate[1], translate[2]);
  glRotatef(rotate[0], 1.0, 0.0, 0.0);
  glRotatef(rotate[1], 0.0, 1.0, 0.0);
  glRotatef(rotate[2], 0.0, 0.0, 1.0);
  glScalef(scale[0], scale[1], scale[2]);

  glPointSize(1.0);
  drawInscribedPolygon(&lower_base_center, radius, sides, rgb);
  drawInscribedPolygon(&upper_base_center, radius, sides, rgb);
  for (i = 0; i <= sides; i++) {
    angle = 2 * M_PI * i / sides;
    points[0] = getPointOnCircumference(&lower_base_center, radius, angle);
    points[1] = getPointOnCircumference(&upper_base_center, radius, angle);

    angle = 2 * M_PI * (i + 1) / sides;
    points[2] = getPointOnCircumference(&upper_base_center, radius, angle);
    points[3] = getPointOnCircumference(&lower_base_center, radius, angle);

    drawRectangle(points, rgb);
  }

  checkErrors(WINDOW_TITLE);
  glFlush();
}

GLvoid keyboard(unsigned char key, GLint x, GLint y) {
  switch (key) {
  case 'w':
    view[TOP] += 0.1;
    break;
  case 'W':
    view[TOP] -= 0.1;
    break;
  case 's':
    view[BOTTOM] += 0.1;
    break;
  case 'S':
    view[BOTTOM] -= 0.1;
    break;
  case 'a':
    view[LEFT] += 0.1;
    break;
  case 'A':
    view[LEFT] -= 0.1;
    break;
  case 'd':
    view[RIGHT] += 0.1;
    break;
  case 'D':
    view[RIGHT] -= 0.1;
    break;
  case 'q':
    view[NEAR_PLANE] += 0.1;
    break;
  case 'Q':
    view[NEAR_PLANE] -= 0.1;
    break;
  case 'e':
    view[FAR_PLANE] += 0.1;
    break;
  case 'E':
    view[FAR_PLANE] -= 0.1;
    break;
  case 'r':
    view[LEFT] = -1.0;
    view[RIGHT] = 1.0;
    view[BOTTOM] = -1.0;
    view[TOP] = 1.0;
    view[NEAR_PLANE] = 1.0;
    view[FAR_PLANE] = 5.0;
    translate[0] = 0.0;
    translate[1] = 0.0;
    translate[2] = -3.0;
    rotate[0] = 0.0;
    rotate[1] = 0.0;
    rotate[2] = 0.0;
    scale[0] = 1.0;
    scale[1] = 1.0;
    scale[2] = 1.0;
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
    if (state == GLUT_DOWN) {
      rotate[0] += 10.0;
      setProjection();
      glutPostRedisplay();
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      rotate[1] -= 10.0;
      setProjection();
      glutPostRedisplay();
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if (state == GLUT_DOWN) {
      rotate[2] += 10.0;
      setProjection();
      glutPostRedisplay();
    }
    break;
  }
}

GLvoid gluPerspectiveWrapper(GLdouble left, GLdouble right, GLdouble bottom,
                             GLdouble top, GLdouble near_plane,
                             GLdouble far_plane) {
  gluPerspective(45.0, (right - left) / (top - bottom), near_plane, far_plane);
}
