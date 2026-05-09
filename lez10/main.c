#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#define N 4

const Point CP0[N] = {
  {-10.0, 0.0, 0.0}, 
  {-5.0, 10.0, 0.0}, 
  {0.0, 20.0, 0.0}, 
  {5.0, 10.0, 0.0}
};
const Point CP1[N] = { 
  { -4.0, -4.0, 0.0},
  { -2.0,  4.0, 0.0},
  {  2.0, -4.0, 0.0}, 
  {  4.0,  4.0, 0.0}
};
Point CP2[N] = {
  {0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0}
};
Point CP3[N] = { 
  {0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0}, 
  {0.0, 0.0, 0.0}
};
const double w0[N] = {1.0, 1.0, 1.0, 1.0};
const double w[N] ={ 1.0, 5.0, 1.0, 1.0};
int CP2_counter = 0;
int CP3_counter = 0;

Point eyePosition = {0.0, 0.0, -30.0};
const Point lookAtPoint = {0.0, 0.0, 0.0};
const Point upVector = {0.0, 1.0, 0.0};



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

  // glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);
  
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
      if (CP2_counter > 0 && CP2_counter < N && CP3_counter == 0) {
        rationalBezierCurve(CP2, w0, CP2_counter);
      } else if (CP2_counter == N && CP3_counter > 0 && CP3_counter <= N) {
        compositeBezierCurve(CP2, CP2_counter, w0, CP3, CP3_counter, w0);
      }

    glPopMatrix();
  glPopMatrix();
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
      eyePosition = (Point){0.0, 0.0, -30.0};
      break;
    case 0x1B: // ESC
      exit(0);
      break;
  }
  glutPostRedisplay();
}



GLvoid mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    Point mouseCP;
    mouseCP.x = (double)(x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2) * 10.0;
    mouseCP.y = (double)(WINDOW_HEIGHT / 2 - y) / (WINDOW_HEIGHT / 2) * 10.0;
    mouseCP.z = 0.0;

    if (CP2_counter < N - 1) {
      CP2[CP2_counter++] = mouseCP;
    } else if (CP2_counter == N - 1 && CP3_counter == 0) {
      CP2[CP2_counter++] = mouseCP;
      CP3[CP3_counter++] = mouseCP;
    } else if (CP3_counter > 0 && CP3_counter < N) {
      CP3[CP3_counter++] = mouseCP;
    }
    glutPostRedisplay();
  }
}



int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  (void)glutCreateWindow("Function");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
