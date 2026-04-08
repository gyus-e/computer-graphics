#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
1: Modellare un orologio 2D
La lancetta dell'ora avanza a ogni giro completo della lancetta dei minuti
La lancetta dei minuti avanza con il trascinamento del mouse quando il tasto sx
è premuto

2: Cronometro 3D
Tasti START, STOP, RESET
START fa partire il cronometro
STOP lo ferma
RESET lo azzera
lancetta dei secondi avanza ogni secondo
lancetta minuti avanza ogni qualvolta la lancetta dei secondi completa un giro
*/

const int second = 360.0/60.0;
const int minute = 360.0/60.0;
const int hour = 360.0/12.0;

GLdouble secondsRotation = 0.0;
GLdouble minutesRotation = 0.0;
GLdouble hoursRotation = 0.0;

GLdouble speed = 0.1;
int pause = 0;

void display() {
  const int sides = 360;
  const Point center = {0.0, 0.0, 0.0};
  const double radius = 0.5;
  const double rgb[] = {1.0, 1.0, 1.0};

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(2.0);

  drawInscribedPolygon(&center, radius, sides, rgb);
  glColor3d(0.0, 0.0, 0.0);

  glPushMatrix();
  glRotatef(secondsRotation, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3d(center.x, center.y, center.z);
    glVertex3d(0.0, 0.45, 0.0);
  glEnd();
  glPopMatrix();


  glPointSize(4.0);

  glPushMatrix();
  glRotatef(minutesRotation, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3d(center.x, center.y, center.z);
    glVertex3d(0.0, 0.4, 0.0);
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
  int minutes_per_click = 10;
  
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      minutesRotation -= minutes_per_click*minute;
      if ((int) minutesRotation % 360 == 0) {
        hoursRotation -= hour;
      }
      glutPostRedisplay();
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      minutesRotation += minutes_per_click*minute;
      if ((int) minutesRotation % 360 == 0) {
        hoursRotation += hour;
      }
      glutPostRedisplay();
    }
    break;
  }
}

GLvoid timer(GLint value) {
  if (pause) {
    return;
  }
  secondsRotation -= second;
  if ((int) secondsRotation % 360 == 0) {
    minutesRotation -= minute;
    if ((int) minutesRotation % 360 == 0) {
      hoursRotation -= hour;
    }
  }
  glutPostRedisplay();
  glutTimerFunc(speed*1000, timer, 0);
}

GLvoid keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 's':
    pause = 0;
    glutTimerFunc(speed*1000, timer, 0);
    break;
  case 'p':
    pause = 1;
    break;
  case 'r':
    secondsRotation = 0.0;
    minutesRotation = 0.0;
    hoursRotation = 0.0;
    glutPostRedisplay();
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
//   glutMouseFunc(mouse);
//   glutTimerFunc(1000, timer, 0);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}