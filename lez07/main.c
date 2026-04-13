#include "utils.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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
Mostrare numeri sul quadrante.
*/

GLdouble secondsRotation = 0.0;
GLdouble minutesRotation = 0.0;
GLdouble hoursRotation = 0.0;
GLdouble clockRotation = 0.0;

GLdouble speed = 0.1;
int pause = 1;

void display() {
  const int sides = 360;
  const Point center = {0.0, 0.0, 0.0};
  const double radius = 0.5;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPointSize(2.0);
  glRotated(clockRotation, 0, 1, 0);
  draw2DClock(&center, radius, secondsRotation, minutesRotation, hoursRotation, white, black);
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
    if (pause == 1) {
        pause = 0;
        glutTimerFunc(speed*1000, timer, 0);
    }
    break;
  case 'p':
    pause = 1;
    break;
  case 'r':
    secondsRotation = 0.0;
    minutesRotation = 0.0;
    hoursRotation = 0.0;
    pause = 1;
    glutPostRedisplay();
    break;
  case 'a':
    clockRotation += 20.0;
    glutPostRedisplay();
    break;
  case 'd':
    clockRotation -= 20.0;
    glutPostRedisplay();
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
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