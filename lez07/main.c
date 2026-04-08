#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
Modellare un orologio 2D
La lancetta dell'ora avanza a ogni giro completo della lancetta dei minuti
La lancetta dei minuti avanza con il trascinamento del mouse quando il tasto sx
è premuto
*/

GLdouble minutesRotation = 0.0;
GLdouble hoursRotation = 0.0;

void display() {
  const int sides = 360;
  const Point center = {0.0, 0.0, 0.0};
  const double radius = 0.5;
  const double rgb[] = {1.0, 1.0, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(1.0);

  drawInscribedPolygon(&center, radius, sides, rgb);
  glColor3d(0.0, 0.0, 0.0);

  glPushMatrix();
  glRotatef(minutesRotation, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3d(center.x, center.y, center.z);
    glVertex3d(0.0, 0.5, 0.0);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glRotatef(hoursRotation, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3d(center.x, center.y, center.z);
    glVertex3d(0.25, 0.0, 0.0);
  glEnd();
  glPopMatrix();

  checkErrors("Orologio 2D");
  glFlush();
}

GLvoid mouse(GLint button, GLint state, GLint x, GLint y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      minutesRotation -= 45.0;
      if ((int) minutesRotation % 360 == 0) {
        hoursRotation -= 30.0;
      }
      glutPostRedisplay();
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      minutesRotation += 45.0;
      if ((int) minutesRotation % 360 == 0) {
        hoursRotation += 30.0;
      }
      glutPostRedisplay();
    }
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(500, 500);
  int window = glutCreateWindow("Orologio 2D");
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}