#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};
const double camDistance = 20.0;
double camAngle[2] = {0.0, 0.0};
Point camPosition = {0.0, 0.0, camDistance};

const double sensitivity = 0.01;
int leftMouseDown = 0;
int rightMouseDown = 0;
int lastMouseX = 0;
int lastMouseY = 0;

const double step = 0.5;

const double domain[2][2] = {
  {-10.0, 10.0},
  {-10.0, 10.0}
};

double k = 0.0;


double f1(const double x, const double z, const double k) {
  return 2*sin(sqrt(x*x + z*z))/sqrt(x*x + z*z);
}



double f2(const double x, const double z, const double k) {
  return 2*sin(sqrt(x*x + z*z) - k*M_PI/32)/(sqrt(x*x + z*z) - k*M_PI/32);
}



void reshape(int width, int height) {
  if (height == 0) height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (double)width / (double)height, 0.1, 50.0);
  glMatrixMode(GL_MODELVIEW);
}



void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  glPointSize(1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glShadeModel(GL_FLAT);

  glPushMatrix();
    gluLookAt(camPosition[X], camPosition[Y], camPosition[Z],
              lookAtPoint[X], lookAtPoint[Y], lookAtPoint[Z],
              upVector[X], upVector[Y], upVector[Z]);

    glPushMatrix();
      glColor3d(0.0, 0.0, 1.0);
      // plotFunction(f1, domain, step, 0);
      plotFunction(f2, domain, step, k);
    glPopMatrix();
  glPopMatrix();
  // glFlush();
  glutSwapBuffers();
}



GLvoid keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'r':
      k = 0.0;
      break;
    case 0x1B: // ESC
      exit(0);
      break;
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
    camAngle[Y] += dy * sensitivity;
    camAngle[X] -= dx * sensitivity;
  }
  camPosition[Y] = lookAtPoint[Y] + camDistance * sin(camAngle[Y]);
  camPosition[X] = lookAtPoint[X] + camDistance * cos(camAngle[Y]) * sin(camAngle[X]);
  camPosition[Z] = lookAtPoint[Z] + camDistance * cos(camAngle[Y]) * cos(camAngle[X]);
  glutPostRedisplay();
}



GLvoid time(int value) {
  k+=0.5;
  glutPostRedisplay();
  glutTimerFunc(1000 / 60, time, 0);
}



int main(int argc, char **argv) {
  glutInit(&argc, argv);
  // glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  (void)glutCreateWindow("Function");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutTimerFunc(0, time, 0);
  glutMainLoop();
  return 0;
}
