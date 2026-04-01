#include "checkerror.h"
#include <math.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>

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

enum {LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR};

typedef struct {
  GLdouble x;
  GLdouble y;
  GLdouble z;
} Point;

GLdouble view[6] = {-1.0, 1.0, -1.0, 1.0, 1.0, 5.0};
GLdouble translate[3] = {0.0, 0.0, -3.0};
GLdouble y_rotation = 0.0;
GLdouble z_rotation = 0.0;

Point getPoint(Point centro, const GLdouble raggio, const GLdouble angle) {
  Point p;
  p.x = centro.x + raggio * cos(angle);
  p.y = centro.y + raggio * sin(angle);
  p.z = centro.z;
  return p;
}

GLvoid drawOctagon(Point centro, const GLdouble raggio, const GLdouble rgb[3]) {
  const GLint sides = 8;
  GLdouble angle, x, y, z;

  glColor3f(rgb[0], rgb[1], rgb[2]);
  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.2, 0.2, -1.5);
    for (int i = 0; i <= sides; i++) {
      angle = 2 * M_PI * i / sides;
      Point p = getPoint(centro, raggio, angle);
      glVertex3f(p.x, p.y, p.z);
    }
  glEnd();
}

GLvoid drawRectangle(const Point points[4], const GLdouble rgb[3]) {
  glColor3f(rgb[0], rgb[1], rgb[2]);
  glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
      glVertex3f(points[i].x, points[i].y, points[i].z);
    }
  glEnd();
}

GLvoid display() {
  const Point centro_base_inf = {0.2, 0.2, -0.5};
  const Point centro_base_sup = {0.2, 0.2, -1.5};
  const GLdouble raggio = 0.5;
  const GLdouble rgb[] = {0.0, 0.0, 1.0};
  Point points[4];
  GLdouble angle, x, y, z;

  glPointSize(1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glRotatef(y_rotation, 0.0, 1.0, 0.0);
  glRotatef(z_rotation, 0.0, 0.0, 1.0);
  drawOctagon(centro_base_inf, raggio, rgb);
  drawOctagon(centro_base_sup, raggio, rgb);
  for (int i = 0; i <= 8; i++) {
    angle = 2 * M_PI * i / 8;
    points[0] = getPoint(centro_base_inf, raggio, angle);
    points[1] = getPoint(centro_base_sup, raggio, angle);

    angle = 2 * M_PI * (i + 1) / 8;
    points[2] = getPoint(centro_base_sup, raggio, angle);
    points[3] = getPoint(centro_base_inf, raggio, angle);

    drawRectangle(points, rgb);
  }

  checkError("ESERCIZIO 1");
  glFlush();
}

GLvoid initWindow() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  // glOrtho(view[0], view[1], view[2], view[3], view[4], view[5]);
  glFrustum(view[0], view[1], view[2], view[3], view[4], view[5]);
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
      view[NEAR] = 1.0;
      view[FAR] = 5.0;
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
