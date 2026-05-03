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

Point eyePosition = {0.0, 0.0, -15.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};

const double step = 0.5;

const double domain[2][2] = {
  {-10.0, 10.0},
  {-10.0, 10.0}
};



double f1(const double x, const double z) {
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
    gluLookAt(eyePosition.x, eyePosition.y, eyePosition.z,
              lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
              upVector.x, upVector.y, upVector.z);

    glPushMatrix();
      glColor3d(0.0, 0.0, 1.0);
      plotFunction(f1, domain, step);
    glPopMatrix();
  glPopMatrix();
  // glFlush();
  glutSwapBuffers();
}



void rotateEyePosition_x(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double x = eyePosition.x * c - eyePosition.z * s;
  double z = eyePosition.x * s + eyePosition.z * c;

  eyePosition.x = x;
  eyePosition.z = z;
}



void rotateEyePosition_y(double angle) {
  double s = sin(angle);
  double c = cos(angle);

  double y = eyePosition.y * c - eyePosition.z * s;
  double z = eyePosition.y * s + eyePosition.z * c;

  eyePosition.y = y;
  eyePosition.z = z;
}



GLvoid keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'a':
      rotateEyePosition_x(-M_PI / 16.0);
      break;
    case 'd':
      rotateEyePosition_x(M_PI / 16.0);
      break;
    case 'w':
      rotateEyePosition_y(M_PI / 16.0);
      break;
    case 's':
      rotateEyePosition_y(-M_PI / 16.0);
      break;
    case 'r':
      eyePosition = (Point){0.0, 0.0, -20.0};
      break;
    case 0x1B: // ESC
      exit(0);
      break;
  }
  glutPostRedisplay();
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
  glutMainLoop();
  return 0;
}
