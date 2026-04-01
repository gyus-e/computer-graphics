#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/freeglut_std.h>
#include "utils.h"

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

GLdouble view[6] = {-1.0, 1.0, -1.0, 1.0, 1.0, 5.0};
GLdouble translate[3] = {0.0, 0.0, -3.0};
GLdouble y_rotation = 0.0;
GLdouble z_rotation = 0.0;


GLvoid display() {
  const Point centro_base_inf = {0.2, 0.2, -0.5};
  const Point centro_base_sup = {0.2, 0.2, -1.5};
  const GLdouble raggio = 0.5;
  const GLdouble rgb[] = {0.0, 0.0, 1.0};
  Point points[4];
  GLdouble angle, x, y, z;
  int i;

  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glRotatef(y_rotation, 0.0, 1.0, 0.0);
  glRotatef(z_rotation, 0.0, 0.0, 1.0);
  drawInscribedPolygon(&centro_base_inf, raggio, 8, rgb);
  drawInscribedPolygon(&centro_base_sup, raggio, 8, rgb);
  for (i = 0; i <= 8; i++) {
    angle = 2 * M_PI * i / 8;
    points[0] = getPointOnCircumference(&centro_base_inf, raggio, angle);
    points[1] = getPointOnCircumference(&centro_base_sup, raggio, angle);

    angle = 2 * M_PI * (i + 1) / 8;
    points[2] = getPointOnCircumference(&centro_base_sup, raggio, angle);
    points[3] = getPointOnCircumference(&centro_base_inf, raggio, angle);

    drawRectangle(points, rgb);
  }

  checkErrors("ESERCIZIO 1");
  glFlush();
}

GLvoid initWindow() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  // glOrtho(view[LEFT], view[RIGHT], view[BOTTOM], view[TOP], view[NEAR_PLANE], view[FAR_PLANE]);
  glFrustum(view[LEFT], view[RIGHT], view[BOTTOM], view[TOP], view[NEAR_PLANE], view[FAR_PLANE]);
  glTranslatef(translate[0], translate[1], translate[2]);
}

void keyboard(unsigned char key, GLint x, GLint y) {
  switch (key) {
    case 'w':
      view[TOP]+=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'W':
      view[TOP]-=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 's':
      view[BOTTOM]+=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'S':
      view[BOTTOM]-=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'a':
      view[LEFT]+=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'A':
      view[LEFT]-=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'd':
      view[RIGHT]+=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'D':
      view[RIGHT]-=0.1;
      initWindow();
      glutPostRedisplay();
      break;
    case 'r':
      view[LEFT] = -1.0;
      view[RIGHT] = 1.0;
      view[BOTTOM] = -1.0;
      view[TOP] = 1.0;
      view[NEAR_PLANE] = 1.0;
      view[FAR_PLANE] = 5.0;
      y_rotation = 0.0;
      initWindow();
      glutPostRedisplay();
      break;
    case 27: // ESC key
      exit(0);
  }
}

GLvoid mouse(GLint button, GLint state, GLint x, GLint y) {
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN) {
        initWindow();
        y_rotation += 30.0;
        glutPostRedisplay();
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if (state == GLUT_DOWN) {
        initWindow();
        y_rotation -= 30.0;
        glutPostRedisplay();
      }
      break;
    case GLUT_MIDDLE_BUTTON:
      if (state == GLUT_DOWN) {
        initWindow();
        z_rotation += 30.0;
        glutPostRedisplay();
      }
      break;
  }
}

int main(int argc, char **argv) {
  const GLint size = 450;
  GLint window;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(size, size);
  window = glutCreateWindow("ESERCIZIO 1");

  initWindow();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
